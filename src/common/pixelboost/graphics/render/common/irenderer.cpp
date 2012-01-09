#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/graphics/render/common/renderer.h"

using namespace pixelboost;

IRenderer::IRenderer(Renderer* renderer)
    : _Renderer(renderer)
{
    _Id = renderer->AddRenderer(this);
}

IRenderer::~IRenderer()
{
    _Renderer->RemoveRenderer(this);
}

int IRenderer::GetId()
{
    return _Id;
}
