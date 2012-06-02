#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"

#include "game.h"

Game::Game(void* viewController)
    : pb::Game(viewController)
{
    _Camera = new pb::OrthographicCamera();
    _Layer = new pb::RenderLayer(0, _Camera);
    
    GetRenderer()->AddLayer(_Layer);
}

Game::~Game()
{
    GetRenderer()->RemoveLayer(_Layer);
}

void Game::Update(float time)
{
    pb::Game::Update(time);
}

void Game::Render()
{
    GetPrimitiveRenderer()->AttachBox(_Layer, Vec2(0,0), Vec2(20,20), Vec3(0,0,0), Vec4(0,0,0,1));
    GetPrimitiveRenderer()->AttachBox(_Layer, Vec2(0,0), Vec2(5,5), Vec3(0,0,0), Vec4(1,1,1,1));
    
    pb::Game::Render();
}
