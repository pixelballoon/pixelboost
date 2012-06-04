#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/components/rectangle.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

#include "game.h"

Game::Game(void* viewController)
    : pb::Game(viewController)
{
    _Scene = new pb::Scene();
    
    pb::Entity* entity = new pb::Entity(0);
    _Scene->AddEntity(entity);
    
    _CameraA = new pb::OrthographicCamera();
    _CameraA->Position = glm::vec3(0, 0, 0);
    _CameraB = new pb::OrthographicCamera();
    _CameraB->Position = glm::vec3(0, 0, 0);
    
    pb::RectangleComponent* rectangle = new pb::RectangleComponent(entity);
    rectangle->SetColor(glm::vec4(1,1,1,1));
    rectangle->SetSize(glm::vec2(5,5));
    entity->AddComponent(rectangle);
    
    glm::vec2 displaySize = pb::GraphicsDevice::Instance()->GetDisplayResolution();
    
    _ViewportA = new pb::Viewport(0, _CameraA);
    _ViewportA->SetResolution(glm::vec2(displaySize.x, displaySize.y/2.f));
    _ViewportA->SetPosition(glm::vec2(0, displaySize.y/4.f));
    _ViewportA->SetScene(_Scene);
    
    _ViewportB = new pb::Viewport(0, _CameraB);
    _ViewportB->SetResolution(glm::vec2(displaySize.x, displaySize.y/2.f));
    _ViewportB->SetPosition(glm::vec2(0, -displaySize.y/4.f));
    _ViewportB->SetScene(_Scene);
    
    pb::Renderer::Instance()->AddViewport(_ViewportA);
    pb::Renderer::Instance()->AddViewport(_ViewportB);
}

Game::~Game()
{
    pb::Renderer::Instance()->RemoveViewport(_ViewportA);
    pb::Renderer::Instance()->RemoveViewport(_ViewportB);
    
    delete _ViewportA;
    delete _ViewportB;
    delete _CameraA;
    delete _CameraB;
}

void Game::Update(float time)
{
    _Scene->Update(time);
    
    pb::Game::Update(time);
}

void Game::Render()
{
    pb::Game::Render();
}
