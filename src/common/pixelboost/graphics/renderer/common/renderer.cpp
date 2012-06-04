#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Renderer* Renderer::Renderer::_Instance = 0;

Renderer::Renderer()
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
    for (RendererSet::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void Renderer::Render()
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        (*it)->Render();
        
        FlushBuffer(*it);
    }
}

void Renderer::AddItem(Renderable* renderable)
{
    _Renderables[renderable->GetLayer()].push_back(renderable);
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

void Renderer::SetHandler(int renderableType, IRenderer* renderer)
{
    _RenderableHandlers[renderableType] = renderer;
}

void Renderer::FlushBuffer(Viewport* viewport)
{
    for (LayerRenderableMap::iterator it = _Renderables.begin(); it != _Renderables.end(); ++it)
    {
        RenderableList& renderables = it->second;
        
        Uid type = _Renderables.size() ? renderables[0]->GetRenderableType() : 0;
        int start = 0;
        int count = 0;
        
        for (int i=0; i < _Renderables.size(); i++)
        {
            Uid newType = renderables[i]->GetRenderableType();
            
            if (type == newType)
            {
                count++;
            } else {
                RenderBatch(viewport, count, renderables[start]);
                start = i;
                count = 1;
                type = newType;
            }
        }
        
        if (count > 0)
        {
            RenderBatch(viewport, count, renderables[start]);
        }
    }
    
    _Renderables.clear();
}

void Renderer::RenderBatch(Viewport* viewport, int count, Renderable* renderable)
{
    RenderableHandlerMap::iterator it = _RenderableHandlers.find(renderable->GetRenderableType());
    
    if (it != _RenderableHandlers.end())
    {
        it->second->Render(count, renderable, viewport);
    }
}

void Renderer::AddRenderer(IRenderer* renderer)
{
    _Renderers.insert(renderer);
}

void Renderer::RemoveRenderer(IRenderer* renderer)
{
    _Renderers.erase(renderer);
}

#endif
