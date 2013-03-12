#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

GuiRenderSystem::GuiRenderSystem()
{
    Engine::Instance()->GetKeyboardManager()->AddHandler(this);
    Engine::Instance()->GetMouseManager()->AddHandler(this);
    
    _State.Active.Item = {0,0,0};
}

GuiRenderSystem::~GuiRenderSystem()
{
    Engine::Instance()->GetKeyboardManager()->RemoveHandler(this);
    Engine::Instance()->GetMouseManager()->RemoveHandler(this);
}

Uid GuiRenderSystem::GetType() const
{
    return GuiRenderSystem::GetStaticType();
}

Uid GuiRenderSystem::GetStaticType()
{
    return TypeHash("pb::GuiRenderSystem");
}

void GuiRenderSystem::Update(Scene* scene, float totalTime, float gameTime)
{
    /*
    if (_InputEvents.size())
    {
        //_State.HotItem = {0,0,0};
    }
    
    for (const auto& gui : _GuiItems)
    {
        //gui->OnLayout(_State, this);
    }
    */
    for (const auto& event : _InputEvents)
    {
        //for (const auto& gui : _GuiItems)
        //{
            if (event.Type == GuiInputEvent::kInputEventMouse)
            {
                if (event.Mouse.Type == MouseEvent::kMouseEventMove)
                {
                    /*
                    glm::mat4x4 projectionMatrix = gui->GetRenderable()->GetRenderPass() == kRenderPassScene ? event.Mouse.Viewport->GetSceneCamera()->ProjectionMatrix : event.Mouse.Viewport->GetUiCamera()->ProjectionMatrix;
                    
                    glm::mat4x4 modelViewMatrix = gui->GetRenderable()->GetModelViewMatrix();
                    glm::vec4 viewportRegion = event.Mouse.Viewport->GetNativeRegion();//glm::vec4(event.Mouse.Viewport->GetPosition(), event.Mouse.Viewport->GetResolution());
                    
                    glm::vec2 screenMouse = glm::vec2(event.Mouse.Move.Position[0], event.Mouse.Move.Position[1]);
                    
                    glm::vec3 rayStart = glm::unProject(glm::vec3(screenMouse, 0.4), modelViewMatrix, projectionMatrix, viewportRegion);
                    glm::vec3 rayEnd = glm::unProject(glm::vec3(screenMouse, 0.6), modelViewMatrix, projectionMatrix, viewportRegion);
                    
                    glm::vec3 mouse = glm::mix(rayStart, rayEnd, rayStart.z / glm::distance(rayStart.z, rayEnd.z));
                    
                    _State.Mouse = glm::vec2(mouse.x, mouse.y);
                    */
                    
                    _State.MousePosition = glm::vec2(event.Mouse.Move.Position[0], event.Mouse.Move.Position[1]);
                }
            }
            
            //gui->OnInput(_State, this, event);
        //}
    }
    
    _InputEvents.clear();
}

void GuiRenderSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    for (const auto& gui : _GuiItems)
    {
        if (!_State.Active.Active)
        {
            _State.Active.Item = {0,0,0};
        }
        
        _State.Active.Active = false;
        _State.Hot.Item = {0,0,0};
        gui->GetRenderable()->ClearCommands();
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeLayout);
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeRender);
        
        _State.MousePressed = false;
        _State.MouseReleased = false;
    }
}

void GuiRenderSystem::AddLayout(GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size)
{
    GuiLayout layout;
    layout.Position = glm::vec2(10,10);
    layout.Size = size;
    _GuiLayout[guiId] = layout;
}

GuiLayout GuiRenderSystem::GetLayout(GuiId guiId)
{
    return _GuiLayout[guiId];
}

void GuiRenderSystem::AddGui(GuiComponent* component)
{
    _GuiItems.insert(component);
}

void GuiRenderSystem::RemoveGui(GuiComponent* component)
{
    _GuiItems.erase(component);
}

int GuiRenderSystem::GetInputHandlerPriority()
{
    return 10;
}

bool GuiRenderSystem::OnMouseEvent(MouseEvent event)
{
    GuiInputEvent inputEvent;
    inputEvent.Type = GuiInputEvent::kInputEventMouse;
    inputEvent.Mouse = event;
    _InputEvents.push_back(inputEvent);
    
    if (event.Type == MouseEvent::kMouseEventDown)
    {
        _State.MouseDown = true;
        _State.MousePressed = true;
    } else if (event.Type == MouseEvent::kMouseEventUp)
    {
        _State.MouseDown = false;
        _State.MouseReleased = true;
    }
    
    return false;
}

bool GuiRenderSystem::OnKeyboardEvent(KeyboardEvent event)
{
    return false;
}
