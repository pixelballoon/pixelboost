#include "libpixel/graphics/camera/camera.h"
#include "libpixel/graphics/render/common/layer.h"
#include "libpixel/graphics/render/common/irenderer.h"

using namespace libpixel;

RenderLayer::RenderLayer(Camera* camera)
    : _Camera(camera)
{
    
}

void RenderLayer::Render()
{
    _Camera->ApplyTransform();
    
    for (RendererList::iterator it = _Renderers.begin(); it != _Renderers.end(); ++it)
    {
        (*it)->Render(this);
    }
}

void RenderLayer::AddRenderer(IRenderer* renderer)
{
    _Renderers.push_back(renderer);
}

void RenderLayer::RemoveRenderer(IRenderer* renderer)
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

const RenderLayer::RendererList& RenderLayer::GetRenderers() const
{
    return _Renderers;
}
