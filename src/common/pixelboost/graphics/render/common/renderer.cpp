#include <algorithm>
#include <set>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/common/layer.h"
#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/graphics/render/common/renderer.h"

using namespace pixelboost;

Renderer::Renderer()
{
    
}

Renderer::~Renderer()
{
    
}

void Renderer::Update(float time)
{
    for (RendererList::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void Renderer::Render()
{
    std::sort(_Layers.begin(), _Layers.end(), &Renderer::LayerSortPredicate);
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        (*it)->_Camera->ApplyTransform();
        for (RendererList::iterator rendererIt = _Renderers.begin(); rendererIt != _Renderers.end(); ++rendererIt)
        {
            (*rendererIt)->Render(*it);
        }
    }
}

void Renderer::AddRenderer(IRenderer* renderer)
{
    _Renderers.push_back(renderer);
}

void Renderer::RemoveRenderer(IRenderer* renderer)
{
    for (RendererList::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        if (*it == renderer)
        {
            _Renderers.erase(it);
            return;
        }
    }
}

void Renderer::AddLayer(RenderLayer* layer)
{
    _Layers.push_back(layer);
}

void Renderer::RemoveLayer(RenderLayer* layer)
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

bool Renderer::LayerSortPredicate(const RenderLayer* a, const RenderLayer* b)
{
    return a->_Depth < b->_Depth;
}
