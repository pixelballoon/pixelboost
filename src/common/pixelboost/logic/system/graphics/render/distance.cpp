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

void DistanceRenderSystem::Render(Scene* scene, Viewport* viewport)
{
    glm::vec3 cameraPosition = viewport->GetCamera()->Position;
    cameraPosition.z = 0;
    
    for (RenderableSet::iterator it = _Renderables.begin(); it != _Renderables.end(); ++it)
    {
        glm::vec4 position = (*it)->GetWorldMatrix()[3];
        
        if (glm::distance(cameraPosition, glm::vec3(position.x, position.y, 0)) < _Distance)
            RenderItem(*it);
    }
}

void DistanceRenderSystem::AddItem(Renderable* renderable)
{
    _Renderables.insert(renderable);
}

void DistanceRenderSystem::RemoveItem(Renderable* renderable)
{
    _Renderables.erase(renderable);
}
