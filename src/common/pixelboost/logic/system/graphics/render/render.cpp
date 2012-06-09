#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Uid RenderSystem::GetType()
{
    return GetStaticType();
}

Uid RenderSystem::GetStaticType()
{
    return TypeHash("render");
}

void RenderSystem::RenderItem(Renderable* renderable)
{
    Renderer::Instance()->AttachRenderable(renderable);
}
