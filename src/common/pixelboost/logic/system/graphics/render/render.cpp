#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/render.h"

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

void RenderSystem::AddItem(Renderable* renderable)
{
    renderable->SetSystem(this);
}

void RenderSystem::RemoveItem(Renderable* renderable)
{
    renderable->SetSystem(0);
}
