#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/render.h"

using namespace pb;

Uid RenderSystem::GetType() const
{
    return GetStaticType();
}

Uid RenderSystem::GetStaticType()
{
    return TypeHash("pb::RenderSystem");
}

void RenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    _RenderFilter = viewport->GetRenderFilter();
}

void RenderSystem::RenderItem(Renderable* renderable)
{
    if (_RenderFilter & renderable->GetGroup())
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

void RenderSystem::UpdateBounds(Renderable* renderable)
{
    
}
