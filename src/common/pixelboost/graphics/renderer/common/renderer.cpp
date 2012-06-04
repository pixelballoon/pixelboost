#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Renderer* Renderer::Renderer::_Instance = 0;

Renderer::Renderer()
{
    _Instance = this;
    
    _EffectManager = new EffectManager();
}

Renderer::~Renderer()
{
    _Instance = 0;
}

Renderer* Renderer::Instance()
{
    return _Instance;
}

void Renderer::Render()
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        (*it)->Render();
        
        FlushBuffer(*it);
    }
}

EffectManager* Renderer::GetEffectManager()
{
    return _EffectManager;
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
        
        if (!renderables.size())
            continue;
        
        Uid type = renderables[0]->GetRenderableType();
        Effect* effect = renderables[0]->GetEffect();
        int start = 0;
        int count = 0;
        
        for (int i=0; i < _Renderables.size(); i++)
        {
            Uid newType = renderables[i]->GetRenderableType();
            Effect* newEffect = renderables[i]->GetEffect();
            
            if (type == newType && effect == newEffect)
            {
                count++;
            } else {
                RenderBatch(viewport, count, renderables[start], effect);
                start = i;
                count = 1;
                type = newType;
                effect = newEffect;
            }
        }
        
        if (count > 0)
        {
            RenderBatch(viewport, count, renderables[start], effect);
        }
    }
    
    _Renderables.clear();
}

void Renderer::RenderBatch(Viewport* viewport, int count, Renderable* renderable, Effect* effect)
{
    if (!effect)
        return;
    
    RenderableHandlerMap::iterator it = _RenderableHandlers.find(renderable->GetRenderableType());
    
    if (it != _RenderableHandlers.end())
    {
        EffectTechnique* technique = effect->GetTechnique(viewport->GetRenderScheme());
        
        if (!technique)
        {
            for (int i=0; i < count; i++)
            {
                technique = viewport->GetTechnique(&renderable[i], effect);
                
                if (technique)
                {
                    for (int j=0; j<technique->GetNumPasses(); j++)
                    {
                        EffectPass* pass = technique->GetPass(j);
                        pass->Bind();
                        it->second->Render(1, &renderable[i], viewport, pass);
                    }
                }
            }
        } else
        {
            for (int i=0; i<technique->GetNumPasses(); i++)
            {
                EffectPass* pass = technique->GetPass(i);
                pass->Bind();
                it->second->Render(count, renderable, viewport, pass);
            }
        }
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
