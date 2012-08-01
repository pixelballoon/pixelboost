#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/logic/system/graphics/render/distance.h"

using namespace pb;

DistanceRenderSystem::DistanceRenderSystem(float distance)
    : _Distance(distance)
{
    
}

Uid DistanceRenderSystem::GetStaticType()
{
    return RenderSystem::GetStaticType();
}

void DistanceRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    if (renderPass == kRenderPassUi)
    {
        for (RenderableSet::iterator it = _UiRenderables.begin(); it != _UiRenderables.end(); ++it)
        {
            RenderItem(*it);
        }
    } else {
        glm::vec3 cameraPosition = viewport->GetSceneCamera()->Position;
        cameraPosition.z = 0;
        
        for (RenderableSet::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
        {
            glm::vec4 position = (*it)->GetWorldMatrix()[3];
            
            if ((*it)->GetRenderableType() == TypeHash("particle")|| glm::distance(cameraPosition, glm::vec3(position.x, position.y, 0)) < _Distance)
                RenderItem(*it);
        }
    }
}

void DistanceRenderSystem::AddItem(Renderable* renderable)
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

void DistanceRenderSystem::RemoveItem(Renderable* renderable)
{
    _SceneRenderables.erase(renderable);
    _UiRenderables.erase(renderable);
    
    RenderSystem::RemoveItem(renderable);
}
