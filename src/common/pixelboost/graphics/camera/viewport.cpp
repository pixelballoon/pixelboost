#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Viewport::Viewport(int viewportId)
    : _ViewportId(viewportId)
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

void Viewport::AddLayer(RenderLayer* layer)
{
    _Layers.push_back(layer);
}

void Viewport::RemoveLayer(RenderLayer* layer)
{
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        if (*it == layer)
        {
            _Layers.erase(it);
            return;
        }
    }
}

void Viewport::Render()
{
    GraphicsDevice::Instance()->SetViewport(glm::vec4(_Position.x + GraphicsDevice::Instance()->GetDisplayResolution().x/2.f - _Resolution.x/2.f, _Position.y + GraphicsDevice::Instance()->GetDisplayResolution().y/2.f - _Resolution.y/2.f, _Resolution.x, _Resolution.y));
    
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        (*it)->GetCamera()->CalculateTransform(this);
        
        for (Renderer::RendererMap::iterator rendererIt = pb::Game::Instance()->GetRenderer()->_Renderers.begin(); rendererIt != pb::Game::Instance()->GetRenderer()->_Renderers.end(); ++rendererIt)
        {
            rendererIt->second->Render(*it);
        }
    }
}
