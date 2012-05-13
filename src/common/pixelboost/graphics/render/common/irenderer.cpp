#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/graphics/render/common/renderer.h"

using namespace pb;

IRenderer::IRenderer()
{
    _Id = Renderer::Instance()->AddRenderer(this);
}

IRenderer::~IRenderer()
{
    Renderer::Instance()->RemoveRenderer(this);
}

int IRenderer::GetId()
{
    return _Id;
}

#endif
