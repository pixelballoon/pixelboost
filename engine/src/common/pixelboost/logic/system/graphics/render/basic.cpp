#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/basic.h"

using namespace pb;

Uid BasicRenderSystem::GetStaticType()
{
    return RenderSystem::GetStaticType();
}

void BasicRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    RenderSystem::Render(scene, viewport, renderPass);
    
    switch (renderPass)
    {
        case kRenderPassUi:
        {
            for (RenderableSet::iterator it = _UiRenderables.begin(); it != _UiRenderables.end(); ++it)
            {
                RenderItem(*it);
            }
            break;
        }
            
        case kRenderPassScene:
        {
            for (RenderableSet::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
            {
                RenderItem(*it);
            }
            break;
        }
    }
}

void BasicRenderSystem::AddItem(Renderable* renderable)
{
    switch (renderable->GetRenderPass())
    {
        case kRenderPassScene:
            _SceneRenderables.insert(renderable);
            break;
        case kRenderPassUi:
            _UiRenderables.insert(renderable);
            break;
    }
    
    RenderSystem::AddItem(renderable);
}

void BasicRenderSystem::RemoveItem(Renderable* renderable)
{
    _SceneRenderables.erase(renderable);
    _UiRenderables.erase(renderable);
    
    RenderSystem::AddItem(renderable);
}

const std::set<Renderable*>& BasicRenderSystem::GetItems(RenderPass pass)
{
    switch (pass)
    {
        case kRenderPassScene:
            return _SceneRenderables;
        case kRenderPassUi:
            return _UiRenderables;
    }
}
