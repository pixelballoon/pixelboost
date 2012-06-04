#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

IRenderer::IRenderer()
{
    Renderer::Instance()->AddRenderer(this);
}

IRenderer::~IRenderer()
{
    Renderer::Instance()->RemoveRenderer(this);
}

#endif
