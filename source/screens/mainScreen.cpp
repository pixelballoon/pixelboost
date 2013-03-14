#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

#include "core/game.h"
#include "screens/mainScreen.h"

class MainGui : public pb::Entity
{
    PB_DECLARE_ENTITY
    
public:
    MainGui(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
        : pb::Entity(scene, parent, creationEntity)
    {
        CreateComponent<pb::TransformComponent>();
        CreateComponent<pb::GuiComponent>();
        
        RegisterMessageHandler<pb::UpdateMessage>(pb::MessageHandler(this, &MainGui::OnUpdate));
        RegisterMessageHandler<pb::GuiRenderMessage>(pb::MessageHandler(this, &MainGui::OnGuiRender));
    }
    
    virtual ~MainGui()
    {
        
    }
    
private:
    void OnUpdate(const pb::Message& message)
    {
        glm::vec3 position = glm::vec3(-pb::GraphicsDevice::Instance()->GetDisplayResolution()/(pb::GraphicsDevice::Instance()->GetDisplayDensity()*2.f), 0.f);
        position.y = -position.y;
        GetComponent<pb::TransformComponent>()->SetPosition(position);
        GetComponent<pb::GuiComponent>()->SetSize(pb::GraphicsDevice::Instance()->GetDisplayResolution());
    }
    
    void OnGuiRender(const pb::Message& message)
    {
        auto guiRenderMessage = message.As<pb::GuiRenderMessage>();
        
        pb::GuiControls::BeginArea(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::Width(300.f), pb::GuiLayoutHint::ExpandHeight()});
        
        if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Refresh"))
        {
            Game::Instance()->GetMainScreen()->RefreshHosts();
        }
        
        pb::GuiControls::BeginHorizontal(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::ExpandWidth(false)});
        
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Clear A", {pb::GuiLayoutHint::ExpandWidth(true)});
        
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Clear B", {pb::GuiLayoutHint::ExpandWidth(true)});
        
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Testing Long", {pb::GuiLayoutHint::ExpandWidth(false)});
        
        /*
        pb::GuiControls::BeginVertical(guiRenderMessage, PbGuiId(guiRenderMessage, 0));
        
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Test A");
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Test B");
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Test C");
        
        pb::GuiControls::EndVertical(guiRenderMessage);
        */
        
        pb::GuiControls::EndHorizontal(guiRenderMessage);

        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Clear Filter", {pb::GuiLayoutHint::ExpandWidth(false)});
        pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Clear Filter");
        
        pb::GuiControls::EndArea(guiRenderMessage);
    }
};

PB_DEFINE_ENTITY(MainGui)

MainScreen::MainScreen()
    : NetworkHandler('dbvr')
    , _Scene(0)
    , _Viewport(0)
{
    pb::GraphicsDevice::Instance()->SetClearColor(glm::vec4(43.f/255.f,43.f/255.f,43.f/255.f,1.f));
    
    Game::Instance()->GetFontRenderer()->LoadFont("helvetica", "/fonts/helvetica");
    
    _DiscoveryClient = pb::NetworkManager::Instance()->ClientDiscover(9091, "pb::debugvariable");

    _Client = 0;
}

MainScreen::~MainScreen()
{
    
}

void MainScreen::Update(float timeDelta, float gameDelta)
{
    if (_Scene)
    {
        _Scene->Update(timeDelta, gameDelta);
        
        auto hosts = _DiscoveryClient->GetHosts();
        
        for (const auto& host : hosts)
        {
            if (!_Client)
            {
                _Client = pb::NetworkManager::Instance()->ClientConnect(host, 9090);
                
                _Client->RegisterHandler(this);
            }
        }
    }
    
    if (_Viewport)
    {
        _Viewport->SetResolution(pb::GraphicsDevice::Instance()->GetDisplayResolution());
    }
}

void MainScreen::SetActive(bool active)
{
    Screen::SetActive(active);
    
    if (active)
    {
        _Camera = new pb::OrthographicCamera();
        _Scene = new pb::Scene();
        _Viewport = new pb::Viewport(0, _Camera);
        _Viewport->SetScene(_Scene);
        
        AddViewport(_Viewport);
        
        _Scene->AddSystem(new pb::BoundsRenderSystem());
        _Scene->AddSystem(new pb::GuiSystem());
        
        AddControls();
    } else {
        DestroyViewport(_Viewport);
        _Viewport = 0;
        delete _Camera;
        _Camera = 0;
        //delete _Scene;
        //_Scene = 0;
    }
}

void MainScreen::RefreshHosts()
{
    _DiscoveryClient->Refresh();
}

void MainScreen::OnConnectionOpened(pb::NetworkConnection& connection)
{
    pb::NetworkMessage message;
    
    message.SetProtocol('dbvs');
    message.WriteString("list");
    
    _Client->BroadcastMessage(message);
}

void MainScreen::OnConnectionClosed(pb::NetworkConnection& connection)
{
    
}

void MainScreen::OnReceive(pb::NetworkConnection& connection, pb::NetworkMessage& message)
{
    const char* command;
    if (!message.ReadString(command))
        return;
    
    if (strcmp(command, "varlist") == 0)
    {
        int numVariables;
        message.ReadInt(numVariables);
        
        for (int i=0; i<numVariables; i++)
        {
            const char* name;
            message.ReadString(name);
            
            int type;
            message.ReadInt(type);
            
            switch (type)
            {
                case pb::DebugVariable::kVariableTypeInteger:
                {
                    int min, max;
                    message.ReadInt(min);
                    message.ReadInt(max);
                    break;
                }
                case pb::DebugVariable::kVariableTypeFloat:
                {
                    float min, max;
                    message.ReadFloat(min);
                    message.ReadFloat(max);
                }
            }
            
            pb::NetworkMessage variableQuery;
            variableQuery.SetProtocol('dbvs');
            variableQuery.WriteString("value");
            variableQuery.WriteString(name);
            connection.Send(variableQuery);
            
            PbLogDebug("pb.gametool", "variable %s of type %d", name, type);
        }
    } else if (strcmp(command, "value") == 0)
    {
        const char* name;
        message.ReadString(name);
        
        int type;
        message.ReadInt(type);
        
        switch (type)
        {
            case pb::DebugVariable::kVariableTypeInteger:
                break;
            case pb::DebugVariable::kVariableTypeFloat:
                break;
            case pb::DebugVariable::kVariableTypeString:
                break;
            case pb::DebugVariable::kVariableTypeBool:
                break;
            case pb::DebugVariable::kVariableTypeColor:
                break;
        }
        
        PbLogDebug("pb.gametool", "variable %s value received", name);
    }
}

void MainScreen::AddControls()
{
    _Scene->CreateEntity<MainGui>(0, 0);
}
