#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

DebugRenderSystem::DebugRenderSystem()
    : _UpdateTime(0)
{
    
}

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

Uid DebugRenderSystem::GetType() const
{
    return DebugRenderSystem::GetStaticType();
}

Uid DebugRenderSystem::GetStaticType()
{
    return pb::TypeHash("pb::DebugRenderSystem");
}

void DebugRenderSystem::Update(Scene* scene, float totalTime, float gameTime)
{
    Clear();
    _UpdateTime = gameTime;
    
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
                it->second -= _UpdateTime;
            }
            break;
        }
            
        case kRenderPassScene:
        {
            for (RenderableMap::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end(); ++it)
            {
                RenderItem(it->first);
                it->second -= _UpdateTime;
            }
            break;
        }
    }
}

PrimitiveRenderableEllipse* DebugRenderSystem::AddEllipse(RenderPass renderPass, int layer, glm::vec3 position, glm::vec3 rotation, glm::vec2 size, glm::vec4 color, float time)
{
    PrimitiveRenderableEllipse* ellipse = new pb::PrimitiveRenderableEllipse(0);
    ellipse->SetRenderPass(renderPass);
    ellipse->SetPosition(position);
    ellipse->SetRotation(rotation);
    ellipse->SetSize(size);
    ellipse->SetColor(color);
    ellipse->SetLayer(layer);
    AddTimedItem(ellipse, time);
    
    return ellipse;
}

PrimitiveRenderableLine* DebugRenderSystem::AddLine(RenderPass renderPass, int layer, glm::vec3 start, glm::vec3 end, glm::vec4 color, float time)
{
    PrimitiveRenderableLine* line = new pb::PrimitiveRenderableLine(0);
    line->SetRenderPass(renderPass);
    line->SetStart(start);
    line->SetEnd(end);
    line->SetColor(color);
    line->SetLayer(layer);
    AddTimedItem(line, time);
    
    return line;
}

FontRenderable* DebugRenderSystem::AddText(RenderPass renderPass, int layer, FontAlign alignment, const std::string font, const std::string text, glm::vec3 position, glm::vec3 rotation, float size, float time)
{
    FontRenderable* renderable = new pb::FontRenderable(0);
    renderable->SetRenderPass(renderPass);
    renderable->SetLayer(layer);
    renderable->SetSize(size);
    glm::mat4x4 transform;
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, rotation.x, glm::vec3(1,0,0));
    transform = glm::rotate(transform, rotation.y, glm::vec3(0,1,0));
    transform = glm::rotate(transform, rotation.z, glm::vec3(0,0,1));
    renderable->SetTransform(transform);
    renderable->SetText(text);
    renderable->SetFont(font);
    renderable->SetAlignment(alignment);
    AddTimedItem(renderable, time);
    
    return renderable;
}

void DebugRenderSystem::AddTimedItem(Renderable* renderable, float time)
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
    
    RenderSystem::RemoveItem(renderable);
}

void DebugRenderSystem::Clear()
{
    for (RenderableMap::iterator it = _SceneRenderables.begin(); it != _SceneRenderables.end();)
    {
        if (it->second <= 0.f)
        {
            delete it->first;
            _SceneRenderables.erase(it++);
        } else {
            ++it;
        }
    }
    
    for (RenderableMap::iterator it = _UiRenderables.begin(); it != _UiRenderables.end();)
    {
        if (it->second <= 0.f)
        {
            delete it->first;
            _UiRenderables.erase(it++);
        } else {
            ++it;
        }
    }
}
