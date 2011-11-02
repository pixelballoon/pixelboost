#include "libpixel/graphics/render/layer/layer.h"
#include "libpixel/graphics/render/renderer.h"

using namespace libpixel;

RenderLayer::RenderLayer()
{
    
}

void RenderLayer::Render()
{
    for (RendererList::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        (*it)->Render(this);
    }
}

void RenderLayer::AddRenderer(Renderer* renderer)
{
    _Renderers.push_back(renderer);
}

void RenderLayer::RemoveRenderer(Renderer* renderer)
{
    for (RendererList::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        if (renderer == *it)
        {
            _Renderers.erase(it);
            return;
        }
    }
}
