#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"

#include "game.h"

Game::Game(void* viewController)
    : pb::Game(viewController)
{
    _Layer = new pb::RenderLayer(0);
    
    _CameraA = new pb::OrthographicCamera();
    _CameraA->Position = glm::vec3(5, 0, 0);
    _ViewportA = new pb::Viewport(0);
    
    _CameraB = new pb::OrthographicCamera();
    _CameraB->Position = glm::vec3(-5, 0, 0);
    _ViewportB = new pb::Viewport(0);
    
    _ViewportA->AddLayer(_Layer, _CameraA);
    _ViewportB->AddLayer(_Layer, _CameraB);
    
    GetRenderer()->AddViewport(_ViewportA);
    GetRenderer()->AddViewport(_ViewportB);
}

Game::~Game()
{
    GetRenderer()->RemoveViewport(_ViewportA);
    GetRenderer()->RemoveViewport(_ViewportB);
    
    delete _ViewportA;
    delete _ViewportB;
    delete _CameraA;
    delete _CameraB;
}

void Game::Update(float time)
{
    pb::Game::Update(time);
}

void Game::Render()
{
    GetPrimitiveRenderer()->AttachBox(_Layer, Vec2(0,0), Vec2(6,6), Vec3(0,0,0), Vec4(0.5,0.5,0.5,1));
    GetPrimitiveRenderer()->AttachBox(_Layer, Vec2(0,0), Vec2(5,5), Vec3(0,0,0), Vec4(1,1,1,1));
    
    pb::Game::Render();
}
