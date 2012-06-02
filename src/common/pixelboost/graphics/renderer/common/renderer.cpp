#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Renderer* Renderer::Renderer::_Instance = 0;

Renderer::Renderer()
    : _FreeRendererId(0)
{
    _Instance = this;
}

Renderer::~Renderer()
{
    _Instance = 0;
}

Renderer* Renderer::Instance()
{
    return _Instance;
}

void Renderer::Update(float time)
{
    for (RendererMap::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        it->second->Update(time);
    }
}

void Renderer::Render()
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        (*it)->Render();
    }
}

int Renderer::AddRenderer(IRenderer* renderer)
{
    int id = _FreeRendererId++;
    _Renderers[id] = renderer;
    return id;
}

void Renderer::RemoveRenderer(IRenderer* renderer)
{
    _Renderers.erase(_Renderers.find(renderer->GetId()));
}

void Renderer::AddViewport(Viewport* viewport)
{
    _Viewports.push_back(viewport);
}

void Renderer::RemoveViewport(Viewport* viewport)
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        if (*it == viewport)
        {
            _Viewports.erase(it);
            return;
        }
    }
}

#endif
