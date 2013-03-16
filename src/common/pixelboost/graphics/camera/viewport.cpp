#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Viewport::Viewport(int viewportId, Camera* camera)
    : _SceneCamera(camera)
    , _Scene(0)
    , _ViewportId(viewportId)
{
    SetRenderScheme(TypeHash("default"));
    SetResolution(GraphicsDevice::Instance()->GetDisplayResolution());
    SetDensity(GraphicsDevice::Instance()->GetDisplayDensity());
    
    _UiCamera = new pb::OrthographicCamera();
}

Viewport::~Viewport()
{
    delete _UiCamera;
}

glm::vec4 Viewport::GetNativeRegion()
{
    return glm::vec4(_Position.x + GraphicsDevice::Instance()->GetDisplayResolution().x/2.f - _Resolution.x/2.f, _Position.y + GraphicsDevice::Instance()->GetDisplayResolution().y/2.f - _Resolution.y/2.f, _Resolution.x, _Resolution.y);
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

void Viewport::SetSceneCamera(Camera* camera)
{
    _SceneCamera = camera;
}

Camera* Viewport::GetSceneCamera()
{
    return _SceneCamera;
}

OrthographicCamera* Viewport::GetUiCamera()
{
    return _UiCamera;
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

void Viewport::Render(RenderPass renderPass)
{
    if (_Scene)
    {
        GraphicsDevice::Instance()->SetViewport(GetNativeRegion());
        
        switch (renderPass)
        {
            case kRenderPassScene:
                if (!_SceneCamera)
                    return;
                _SceneCamera->CalculateTransform(this);
                break;
            case kRenderPassUi:
            {
                if (!_UiCamera)
                    return;
                _UiCamera->CalculateTransform(this);
                break;
            }
        }
        
        _Scene->Render(this, renderPass);
    }
}
