#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Viewport::Viewport(int viewportId, Camera* camera)
    : _Camera(camera)
    , _Scene(0)
    , _ViewportId(viewportId)
{
    SetResolution(GraphicsDevice::Instance()->GetDisplayResolution());
    SetDensity(GraphicsDevice::Instance()->GetDisplayDensity());
}

Viewport::~Viewport()
{
    
}

glm::vec2 Viewport::GetSize()
{
    return GetResolution()/GetDensity();
}

void Viewport::SetResolution(glm::vec2 resolution)
{
    _Resolution = resolution;
}

glm::vec2 Viewport::GetResolution()
{
    return _Resolution;
}

void Viewport::SetDensity(float density)
{
    _Density = density;
}

float Viewport::GetDensity()
{
    return _Density;
}

void Viewport::SetPosition(glm::vec2 position)
{
    _Position = position;
}

glm::vec2 Viewport::GetPosition()
{
    return _Position;
}

Camera* Viewport::GetCamera()
{
    return _Camera;
}

void Viewport::SetRenderScheme(Uid scheme)
{
    _RenderScheme = scheme;
}

Uid Viewport::GetRenderScheme()
{
    return _RenderScheme;
}

void Viewport::SetScene(Scene* scene)
{
    _Scene = scene;
}

Scene* Viewport::GetScene()
{
    return _Scene;
}

void Viewport::Render()
{
    GraphicsDevice::Instance()->SetViewport(glm::vec4(_Position.x + GraphicsDevice::Instance()->GetDisplayResolution().x/2.f - _Resolution.x/2.f, _Position.y + GraphicsDevice::Instance()->GetDisplayResolution().y/2.f - _Resolution.y/2.f, _Resolution.x, _Resolution.y));
    
    _Camera->CalculateTransform(this);
    
    _Scene->Render(this);
}
