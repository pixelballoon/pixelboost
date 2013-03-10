#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/scene.h"

#include "screens/gameScreen.h"

GameScreen::GameScreen()
    : _Scene(0)
    , _Viewport(0)
{
    
}

GameScreen::~GameScreen()
{
    
}

void GameScreen::Update(float timeDelta, float gameDelta)
{
    if (_Scene)
    {
        _Scene->Update(timeDelta, gameDelta);
    }
}

void GameScreen::SetActive(bool active)
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
    } else {
        DestroyViewport(_Viewport);
        _Viewport = 0;
        delete _Camera;
        _Camera = 0;
        delete _Scene;
        _Scene = 0;
    }
}
