#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

DebugRenderSystem::~DebugRenderSystem()
{
    for (RenderableMap::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
    {
        delete it->first;
    }
    
    for (RenderableMap::iterator it = _UiRenderables.begin(); it != _UiRenderables.end(); ++it)
    {
        delete it->first;
    }
}

Uid DebugRenderSystem::GetType()
{
    return DebugRenderSystem::GetStaticType();
}

Uid DebugRenderSystem::GetStaticType()
{
    return pb::TypeHash("pb::DebugRenderSystem");
}

void DebugRenderSystem::Update(Scene* scene, float time)
{
    Clear(time);

    DebugRenderMessage debugRenderMessage(this);
    scene->BroadcastMessage(debugRenderMessage);
}

void DebugRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    switch (renderPass)
    {
        case kRenderPassUi:
        {
            for (RenderableMap::iterator it = _UiRenderables.begin(); it != _UiRenderables.end(); ++it)
            {
                RenderItem(it->first);
            }
            break;
        }
            
        case kRenderPassScene:
        {
            for (RenderableMap::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
            {
                RenderItem(it->first);
            }
            break;
        }
    }
}

pb::Renderable* DebugRenderSystem::AddEllipse(RenderPass, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, float time)
{
    pb::PrimitiveRenderableEllipse* ellipse = new pb::PrimitiveRenderableEllipse(0);
    ellipse->SetPosition(position);
    ellipse->SetRotation(rotation);
    ellipse->SetSize(size);
    AddItem(ellipse, time);
    
    return ellipse;
}

pb::Renderable* DebugRenderSystem::AddLine(RenderPass renderPass, glm::vec3 start, glm::vec3 end, float time)
{
    pb::PrimitiveRenderableLine* line = new pb::PrimitiveRenderableLine(0);
    line->SetRenderPass(renderPass);
    line->SetStart(start);
    line->SetEnd(end);
    AddItem(line, time);
    
    return line;
}

void DebugRenderSystem::AddItem(Renderable* renderable, float time)
{
    switch (renderable->GetRenderPass())
    {
        case kRenderPassScene:
            _SceneRenderables[renderable] = time;
            break;
        case kRenderPassUi:
            _UiRenderables[renderable] = time;
            break;
    }
    
    RenderSystem::AddItem(renderable);
}

void DebugRenderSystem::RemoveItem(Renderable* renderable)
{
    _SceneRenderables.erase(renderable);
    _UiRenderables.erase(renderable);
    
    RenderSystem::AddItem(renderable);
}

void DebugRenderSystem::Clear(float time)
{
    for (RenderableMap::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
    {
        it->second -= time;
        if (it->second <= 0.f)
            delete it->first;
    }
    
    for (RenderableMap::iterator it = _UiRenderables.begin(); it != _UiRenderables.end(); ++it)
    {
        it->second -= time;
        if (it->second <= 0.f)
            delete it->first;
    }
    
    _SceneRenderables.clear();
    _UiRenderables.clear();
}
