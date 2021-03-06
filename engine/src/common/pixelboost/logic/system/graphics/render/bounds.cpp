#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"

using namespace pb;

Uid BoundsRenderSystem::GetStaticType()
{
    return RenderSystem::GetStaticType();
}

void BoundsRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
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
            Camera* camera = viewport->GetSceneCamera();
            const BoundingFrustum& frustum = camera->Frustum;
            for (RenderableSet::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
            {
                if (_RenderFilter & (*it)->GetGroup())
                {
                    const BoundingSphere& bounds = (*it)->GetBounds();
                    if (!bounds.IsValid() || frustum.Intersects(bounds))
                        RenderItem(*it);
                }
            }
            break;
        }
    }
}

void BoundsRenderSystem::AddItem(Renderable* renderable)
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

void BoundsRenderSystem::RemoveItem(Renderable* renderable)
{
    _SceneRenderables.erase(renderable);
    _UiRenderables.erase(renderable);
    
    RenderSystem::AddItem(renderable);
}

const std::set<Renderable*>& BoundsRenderSystem::GetItems(RenderPass pass)
{
    switch (pass)
    {
        case kRenderPassScene:
            return _SceneRenderables;
        case kRenderPassUi:
            return _UiRenderables;
    }
}
