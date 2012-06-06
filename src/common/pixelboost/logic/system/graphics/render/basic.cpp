#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/basic.h"

using namespace pb;

Uid BasicRenderSystem::GetStaticType()
{
    return RenderSystem::GetStaticType();
}

void BasicRenderSystem::Render(Scene* scene, Viewport* viewport)
{
    for (RenderableSet::iterator it = _Renderables.begin(); it != _Renderables.end(); ++it)
    {
        Renderer::Instance()->AddItem(*it);
    }
}

void BasicRenderSystem::AddItem(Renderable* renderable)
{
    _Renderables.insert(renderable);
}

void BasicRenderSystem::RemoveItem(Renderable* renderable)
{
    _Renderables.erase(renderable);
}
