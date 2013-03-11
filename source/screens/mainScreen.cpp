#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

#include "core/game.h"
#include "screens/mainScreen.h"

class MainInputHandler : public pb::MouseHandler, public pb::TouchHandler
{
public:
    MainInputHandler()
    {
        
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 0;
    }
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        return false;
    }
    
    virtual bool OnTouchDown(pb::Touch touch)
    {
        return false;
    }
};

class MainGui : public pb::Entity
{
    PB_DECLARE_ENTITY
    
public:
    MainGui(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
        : pb::Entity(scene, parent, creationEntity)
    {
        CreateComponent<pb::TransformComponent>();
        CreateComponent<pb::GuiComponent>();
        
        RegisterMessageHandler<pb::GuiRenderMessage>(pb::MessageHandler(this, &MainGui::OnGuiRender));
    }
    
    virtual ~MainGui()
    {
        
    }
    
private:
    void OnGuiRender(const pb::Message& message)
    {
        auto guiRenderMessage = message.As<pb::GuiRenderMessage>();
        
        auto guiSystem = guiRenderMessage.GetGuiRenderSystem();
        auto guiComponent = guiRenderMessage.GetGuiComponent();
        
        guiSystem->DoButton(guiComponent, glm::vec2(0,0), glm::vec2(4,1));
        guiSystem->DoButton(guiComponent, glm::vec2(0,1.5), glm::vec2(4,1));
    }
};

PB_DEFINE_ENTITY(MainGui)

MainScreen::MainScreen()
    : NetworkHandler('dbvr')
    , _Scene(0)
    , _Viewport(0)
{
    _InputHandler = new MainInputHandler();
    
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
}

void MainScreen::SetActive(bool active)
{
    Screen::SetActive(active);
    
    if (active)
    {
        Game::Instance()->GetTouchManager()->AddHandler(_InputHandler);
        
        _Camera = new pb::OrthographicCamera();
        _Scene = new pb::Scene();
        _Viewport = new pb::Viewport(0, _Camera);
        _Viewport->SetScene(_Scene);
        
        AddViewport(_Viewport);
        
        _Scene->AddSystem(new pb::BoundsRenderSystem());
        _Scene->AddSystem(new pb::GuiRenderSystem());
        
        AddControls();
    } else {
        Game::Instance()->GetTouchManager()->RemoveHandler(_InputHandler);
        
        DestroyViewport(_Viewport);
        _Viewport = 0;
        delete _Camera;
        _Camera = 0;
        //delete _Scene;
        //_Scene = 0;
    }
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
