#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/scene.h"

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
    : _Scene(0)
    , _Viewport(0)
{
    _InputHandler = new MenuInputHandler();
}

MenuScreen::~MenuScreen()
{
    
}

void MenuScreen::Update(float timeDelta, float gameDelta)
{
    if (_Scene)
    {
        _Scene->Update(timeDelta, gameDelta);
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
        delete _Scene;
        _Scene = 0;
    }
}

void MenuScreen::AddControls()
{
    _Scene->CreateEntity<TitleText>(0, 0);
}
