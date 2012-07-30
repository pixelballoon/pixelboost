#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/quadtree.h"

using namespace pb;

Uid QuadtreeRenderSystem::GetStaticType()
{
    return RenderSystem::GetStaticType();
}

void QuadtreeRenderSystem::Render(Scene* scene, Viewport* viewport)
{
    for (RenderableSet::iterator it = _Renderables.begin(); it != _Renderables.end(); ++it)
    {
        RenderItem(*it);
    }
}

void QuadtreeRenderSystem::AddItem(Renderable* renderable)
{
    _Renderables.insert(renderable);
}

void QuadtreeRenderSystem::RemoveItem(Renderable* renderable)
{
    _Renderables.erase(renderable);
}
