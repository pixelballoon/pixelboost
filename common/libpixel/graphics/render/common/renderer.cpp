#include <algorithm>
#include <set>

#include "libpixel/graphics/render/common/layer.h"
#include "libpixel/graphics/render/common/irenderer.h"
#include "libpixel/graphics/render/common/renderer.h"

using namespace libpixel;

Renderer::InternalRenderLayer::InternalRenderLayer(int depth, RenderLayer* layer)
    : depth(depth)
    , layer(layer)
{

}

Renderer::Renderer()
{
    
}

Renderer::~Renderer()
{
    
}

void Renderer::Update(float time)
{
    std::set<IRenderer*> renderers;
    
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        const RenderLayer::RendererList& rendererList = it->layer->GetRenderers();
        
        for (RenderLayer::RendererList::const_iterator rendererIt = rendererList.begin(); rendererIt != rendererList.end(); ++rendererIt)
        {
            renderers.insert(*rendererIt);
        }
    }
    
    for (std::set<IRenderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void Renderer::Render()
{
    std::sort(_Layers.begin(), _Layers.end(), &Renderer::LayerSortPredicate);
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        it->layer->Render();
    }
}

void Renderer::AddLayer(int depth, RenderLayer* layer)
{
    _Layers.push_back(InternalRenderLayer(depth, layer));
}

void Renderer::RemoveLayer(RenderLayer* layer)
{
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        if (it->layer == layer)
        {
            _Layers.erase(it);
            return;
        }
    }
}

void Renderer::SetLayerDepth(const RenderLayer* layer, int depth)
{
    for (LayerList::iterator it = _Layers.begin(); it != _Layers.end(); ++it)
    {
        if (it->layer == layer)
        {
            it->depth = depth;
            return;
        }
    }
}

bool Renderer::LayerSortPredicate(const InternalRenderLayer& a, const InternalRenderLayer& b)
{
    return a.depth < b.depth;
}
