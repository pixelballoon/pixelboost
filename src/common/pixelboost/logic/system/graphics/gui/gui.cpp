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

GuiLayout::GuiLayout()
{
    PendingPosition = false;
    PendingSize = false;
    HasMinMax = false;
    Expand[0] = Expand[1] = false;
}

void GuiLayout::ProcessHints(const std::vector<GuiLayoutHint>& hints, glm::vec2 size)
{
    if (Parent->LayoutType == GuiLayout::kLayoutTypeVertical)
    {
        Expand[0] = true;
    }
    
    for (const auto& hint : hints)
    {
        switch (hint.Type)
        {
            case GuiLayoutHint::kTypeFixedWidth:
            {
                Expand[0] = false;
                size.x = hint.Float;
                break;
            }
            case GuiLayoutHint::kTypeFixedHeight:
            {
                Expand[0] = false;
                size.y = hint.Float;
                break;
            }
            case GuiLayoutHint::kTypeExpandWidth:
            {
                Expand[0] = hint.Bool;
                break;
            }
            case GuiLayoutHint::kTypeExpandHeight:
            {
                Expand[1] = hint.Bool;
                break;
            }
            default:
                break;
        }
    }
    
    if (Expand[0] == true)
    {
        size.x = -1;
    }
    if (Expand[1] == true)
    {
        size.y = -1;
    }
    
    Size = size;
    PendingSize = (size.x < 0 || size.y < 0);
}

GuiSystem::GuiSystem()
{
    Engine::Instance()->GetKeyboardManager()->AddHandler(this);
    Engine::Instance()->GetMouseManager()->AddHandler(this);
    
    _State.Skin.Font = "helvetica";
    _State.Skin.Padding = glm::vec2(5,5);
    _State.Active.Item = {0,0,0};
}

GuiSystem::~GuiSystem()
{
    Engine::Instance()->GetKeyboardManager()->RemoveHandler(this);
    Engine::Instance()->GetMouseManager()->RemoveHandler(this);
}

Uid GuiSystem::GetType() const
{
    return GuiSystem::GetStaticType();
}

Uid GuiSystem::GetStaticType()
{
    return TypeHash("pb::GuiSystem");
}

void GuiSystem::Update(Scene* scene, float totalTime, float gameTime)
{
    for (const auto& event : _InputEvents)
    {
//      for (const auto& gui : _GuiItems)
        {
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
        }
    }
    
    _InputEvents.clear();
}

void GuiSystem::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    for (const auto& gui : _GuiItems)
    {
        if (!_State.Active.Active)
        {
            _State.Active.Item = {0,0,0};
        }
        
        Clear();
        
        GuiLayout rootLayout;
        rootLayout.Parent = 0;
        rootLayout.LayoutType = GuiLayout::kLayoutTypeVertical;
        rootLayout.Position = glm::vec2(0,0);
        rootLayout.Size = gui->GetSize();
        
        GuiLayout* root = new GuiLayout(rootLayout);
        
        _State.LayoutStack.push_back(root);
        
        _State.Active.Active = false;
        _State.Hot.Item = {0,0,0};
        gui->GetRenderable()->ClearCommands();
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeLayout);
        
        ProcessLayouts();
        
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeInput);
        
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeRender);
        
        _State.MousePressed = false;
        _State.MouseReleased = false;
    }
}

void GuiSystem::PushLayoutArea(const GuiLayout& area, GuiId guiId, const std::vector<GuiLayoutHint> hints)
{
    GuiLayout* parent = _State.LayoutStack.back();
    GuiLayout* layout = new GuiLayout(area);
    
    parent->Children.push_back(layout);
    layout->PendingPosition = true;
    layout->Parent = parent;
    
    _State.LayoutStack.push_back(layout);
    
    layout->ProcessHints(hints, glm::vec2(-1,-1));
    
    PbAssert(_GuiArea.find(guiId) == _GuiArea.end());
    
    _GuiArea[guiId] = layout;
}

GuiLayout* GuiSystem::PopLayoutArea()
{
    GuiLayout* top = _State.LayoutStack.back();
    _State.LayoutStack.pop_back();
    
    return top;
}

GuiLayout* GuiSystem::GetLayoutArea(GuiId guiId)
{
    return _GuiArea[guiId];
}

void GuiSystem::AddLayout(GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size)
{
    GuiLayout* area = _State.LayoutStack.back();
    
    GuiLayout* layout = new GuiLayout();

    layout->Parent = area;
    layout->Size = size;
    layout->PendingPosition = true;
    
    layout->ProcessHints(hints, size);
    
    PbAssert(_GuiLayout.find(guiId) == _GuiLayout.end());
        
    area->Children.push_back(layout);
    _GuiLayout[guiId] = layout;
}

GuiLayout* GuiSystem::GetLayout(GuiId guiId)
{
    return _GuiLayout[guiId];
}

void GuiSystem::ProcessLayouts()
{
    PbLogDebug("pb.gui", "process layout");
    
    bool processing = true;
    do
    {
        PbLogDebug("pb.gui", "pass");
        processing = ProcessLayout(_State.LayoutStack.front(), _State.LayoutStack.front()->Position).Pending;
    } while (processing);
}

GuiSystem::LayoutResult GuiSystem::ProcessLayout(GuiLayout* layout, glm::vec2 position)
{
    glm::vec2 size;
    
    if (layout->PendingPosition && !layout->Parent->PendingPosition && !layout->Parent->PendingSize)
    {
        layout->Position = layout->Parent->Position + position;
        layout->PendingPosition = false;
    }
    
    if (layout->PendingPosition)
    {
        position.x = -1.f;
    }
    
    layout->RemainingSize = layout->Size;
    layout->ChildrenPending = layout->Children.size();
    
    for (const auto& child : layout->Children)
    {
        LayoutResult result = ProcessLayout(child, position);
        
        if (!result.Pending)
        {
            layout->RemainingSize -= result.Size;
            layout->ChildrenPending--;
            
            if (layout->LayoutType == GuiLayout::kLayoutTypeHorizontal)
            {
                position.x += result.Size.x;
            } else {
                position.y += result.Size.y;
            }
        }
    }
    
    if (layout->PendingSize)
    {
        if (!layout->Parent->PendingSize)
        {
            if (layout->Size.x < 0.f)
            {
                if (layout->Expand[0])
                {
                    layout->Size.x = layout->Parent->Size.x;
                }

                layout->PendingSize = false;
            }
            
            if (layout->Size.y < 0.f)
            {
                if (layout->Expand[1])
                {
                    layout->Size.y = layout->Parent->Size.y;
                }
                
                layout->PendingSize = false;
            }
        }
    }
    
    LayoutResult result;
    result.Pending = layout->ChildrenPending > 0 || layout->PendingSize || layout->PendingPosition;
    result.Size = layout->Size;
    return result;
}

void GuiSystem::Clear()
{
    if (_State.LayoutStack.size() == 0)
    {
        return;
    }
    
    while (_State.LayoutStack.size() > 1)
    {
        _State.LayoutStack.pop_back();
    }
    
    PbAssert(_State.LayoutStack.size() == 1);
    
    ClearLayout(_State.LayoutStack.back());
    
    _State.LayoutStack.clear();
    _GuiArea.clear();
    _GuiLayout.clear();
}

void GuiSystem::ClearLayout(GuiLayout* layout)
{
    for (const auto& child : layout->Children)
    {
        ClearLayout(child);
    }
    
    delete layout;
}

void GuiSystem::AddGui(GuiComponent* component)
{
    _GuiItems.insert(component);
}

void GuiSystem::RemoveGui(GuiComponent* component)
{
    _GuiItems.erase(component);
}

int GuiSystem::GetInputHandlerPriority()
{
    return 10;
}

bool GuiSystem::OnMouseEvent(MouseEvent event)
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

bool GuiSystem::OnKeyboardEvent(KeyboardEvent event)
{
    return false;
}
