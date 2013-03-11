#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

#include "core/game.h"
#include "screens/menuScreen.h"

class MenuInputHandler : public pb::MouseHandler, public pb::TouchHandler
{
public:
    MenuInputHandler()
    {
        
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 0;
    }
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        Game::Instance()->SetMode(kGameModeGame);
        return false;
    }
    
    virtual bool OnTouchDown(pb::Touch touch)
    {
        Game::Instance()->SetMode(kGameModeGame);
        return false;
    }
};

class TitleText : public pb::Entity
{
    PB_DECLARE_ENTITY
    
public:
    TitleText(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
        : pb::Entity(scene, parent, creationEntity)
    {
        pb::RectangleComponent* rect = CreateComponent<pb::RectangleComponent>();
        rect->SetSize(glm::vec2(5,1));
        rect->SetSolid(true);
    }
    
    virtual ~TitleText()
    {
        
    }
};

PB_DEFINE_ENTITY(TitleText)

MenuScreen::MenuScreen()
    : NetworkHandler('dbvr')
    , _Scene(0)
    , _Viewport(0)
{
    _InputHandler = new MenuInputHandler();
    
    _DiscoveryClient = pb::NetworkManager::Instance()->ClientDiscover(9091, "pb::debugvariable");

    _Client = 0;
}

MenuScreen::~MenuScreen()
{
    
}

void MenuScreen::Update(float timeDelta, float gameDelta)
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

void MenuScreen::SetActive(bool active)
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

void MenuScreen::OnConnectionOpened(pb::NetworkConnection& connection)
{
    pb::NetworkMessage message;
    
    message.SetProtocol('dbvs');
    message.WriteString("list");
    
    _Client->BroadcastMessage(message);
}

void MenuScreen::OnConnectionClosed(pb::NetworkConnection& connection)
{
    
}

void MenuScreen::OnReceive(pb::NetworkConnection& connection, pb::NetworkMessage& message)
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

void MenuScreen::AddControls()
{
    _Scene->CreateEntity<TitleText>(0, 0);
}
