#include <algorithm>
#include <set>

#include "libpixel/graphics/camera/camera.h"
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
        for (RendererList::iterator rendererIt = _Renderers.begin(); rendererIt != _Renderers.end(); ++rendererIt)
        {
            it->layer->_Camera->ApplyTransform();
            (*rendererIt)->Render(it->layer);
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
