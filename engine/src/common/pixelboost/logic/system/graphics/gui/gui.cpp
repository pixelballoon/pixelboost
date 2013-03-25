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

GuiLayout::GuiLayout(const std::string& tag)
{
    Valid = false;
    Parent = 0;
    Tag = tag;
    MinSize = glm::vec2(-1,-1);
    MaxSize = glm::vec2(-1,-1);
    Expand[0] = Expand[1] = false;
    Size = glm::vec2(-1,-1);
    ContentsSize = glm::vec2(-1,-1);
    LocalPosition = glm::vec2(-1,-1);
    Position = glm::vec2(-1,-1);
    Scroll = glm::vec2(0,0);
    LayoutType = kLayoutTypeHorizontal;
}

void GuiLayout::ProcessHints(const std::vector<GuiLayoutHint>& hints, glm::vec2 size)
{
    MinSize = size;
    
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
                Expand[1] = false;
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
            case GuiLayoutHint::kTypeMinWidth:
            {
                MinSize.x = hint.Float;
                break;
            }
            case GuiLayoutHint::kTypeMinHeight:
            {
                MinSize.y = hint.Float;
                break;
            }
            case GuiLayoutHint::kTypeMaxWidth:
            {
                MaxSize.x = hint.Float;
                break;
            }
            case GuiLayoutHint::kTypeMaxHeight:
            {
                MaxSize.y = hint.Float;
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
        if (event.Type == GuiInputEvent::kInputEventMouse)
        {
            if (event.Mouse.Type == MouseEvent::kMouseEventMove)
            {
                _State.MousePosition = glm::vec2(event.Mouse.Move.Position[0], event.Mouse.Move.Position[1]);
            }
        }
        
        if (event.Type == GuiInputEvent::kInputEventKeyboard)
        {
            if (event.Keyboard.Type == KeyboardEvent::kKeyboardEventDown)
            {
                _State.KeyboardEvent = event.Keyboard;
            }
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
        
        if (!_State.Keyboard.Active)
        {
            _State.Keyboard.Item = {0,0,0};
        }
        
        GuiLayout rootLayout("root");
        rootLayout.Parent = 0;
        rootLayout.LayoutType = GuiLayout::kLayoutTypeVertical;
        rootLayout.Position = glm::vec2(0,0);
        rootLayout.Size = gui->GetSize();
        
        GuiLayout* root = new GuiLayout(rootLayout);
        
        _State.LayoutStack.push_back(root);
        
        _State.Active.Active = false;
        _State.Hot.Item = {0,0,0};
        _State.Keyboard.Active = false;
        gui->GetRenderable()->ClearCommands();
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeLayout);
        
        ProcessLayouts();
        
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeInput);
        
        gui->OnGui(_State, this, GuiRenderMessage::kEventTypeRender);
        
        _State.MousePressed = false;
        _State.MouseReleased = false;
        _State.KeyboardEvent.Type = KeyboardEvent::kKeyboardEventUp;
        
        Clear();
    }
}

void GuiSystem::PushLayoutArea(const GuiLayout& area, GuiId guiId, const std::vector<GuiLayoutHint> hints)
{
    GuiLayout* parent = _State.LayoutStack.back();
    GuiLayout* layout = new GuiLayout(area);
    
    parent->Children.push_back(layout);

    layout->Parent = parent;
    layout->ProcessHints(hints, glm::vec2(-1,-1));
    
    _State.LayoutStack.push_back(layout);
    
    PbAssert(_GuiArea.find(guiId) == _GuiArea.end());
    
    _GuiLayout[guiId] = layout;
}

GuiLayout* GuiSystem::PopLayoutArea()
{
    GuiLayout* top = _State.LayoutStack.back();
    _State.LayoutStack.pop_back();
    
    return top;
}

void GuiSystem::AddLayout(const std::string& tag, GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size)
{
    GuiLayout* parent = _State.LayoutStack.back();
    GuiLayout* layout = new GuiLayout(tag);

    parent->Children.push_back(layout);

    layout->Parent = parent;
    layout->ProcessHints(hints, size);
    
    PbAssert(_GuiLayout.find(guiId) == _GuiLayout.end());

    _GuiLayout[guiId] = layout;
}

GuiLayout* GuiSystem::GetLayout(GuiId guiId)
{
    return _GuiLayout[guiId];
}

GuiData& GuiSystem::GetData(GuiId guiId)
{
    auto it = _GuiData.find(guiId);
    
    if (it != _GuiData.end())
    {
        it->second.Active = true;
        return it->second;
    }

    GuiData& data = _GuiData[guiId];
    data.Active = true;
    data.Initialised = false;

    return data;
}

void GuiSystem::ProcessLayouts()
{
    for (int i=0; i<16; i++)
    {
        if (ProcessLayout(_State.LayoutStack.front(), _State.LayoutStack.front()->Position, true))
        {
            return;
        }
    }
}

bool GuiSystem::ProcessLayout(GuiLayout* layout, glm::vec2 position, bool positionValid)
{
    if (layout->Valid)
    {
        return true;
    }
    
    glm::vec2 childrenSize;
    glm::vec2 childrenMinSize;
    
    if (positionValid)
    {
        if (position.x >= 0.f)
        {
            float parentLocalX = 0.f;
            float parentGlobalX = 0.f;
            float parentScrollX = 0.f;
            
            if (layout->Parent)
            {
                parentLocalX = layout->Parent->LocalPosition.x;
                parentGlobalX = layout->Parent->Position.x;
                parentScrollX = layout->Parent->Scroll.x;
            }
            
            layout->LocalPosition.x = position.x + parentLocalX;
            layout->Position.x = position.x + parentGlobalX - parentScrollX;
        }
        
        if (position.y >= 0.f)
        {
            float parentLocalY = 0.f;
            float parentGlobalY = 0.f;
            float parentScrollY = 0.f;
            
            if (layout->Parent)
            {
                parentLocalY = layout->Parent->LocalPosition.y;
                parentGlobalY = layout->Parent->Position.y;
                parentScrollY = layout->Parent->Scroll.y;
            }
            
            layout->LocalPosition.y = position.y + parentLocalY;
            layout->Position.y = position.y + parentGlobalY - parentScrollY;
        }
    }
    
    bool needAnotherPass = false;
    int numProcessedChildrenX = 0;
    int numProcessedChildrenY = 0;
    
    position = glm::vec2(0,0);
    
    for (const auto& child : layout->Children)
    {
        bool result = ProcessLayout(child, position, positionValid);
        
        if (layout->LayoutType == GuiLayout::kLayoutTypeHorizontal)
        {
            childrenMinSize = glm::vec2(childrenMinSize.x + child->MinSize.x, glm::max(childrenMinSize.y, child->MinSize.y));
            
            if (position.x >= 0.f && child->Size.x)
            {
                position.x += child->Size.x;
            }
            
            if (child->Size.x >= 0.f)
            {
                childrenSize.x += child->Size.x;
                numProcessedChildrenX++;
            } else
            {
                positionValid = false;
            }
            
            if (child->Size.y >= 0.f)
            {
                childrenSize.y = glm::max(childrenSize.y, child->Size.y);
                numProcessedChildrenY++;
            } else
            {
                positionValid = false;
            }
        } else if (layout->LayoutType == GuiLayout::kLayoutTypeVertical)
        {
            childrenMinSize = glm::vec2(glm::max(childrenMinSize.x, child->MinSize.x), childrenMinSize.y + child->MinSize.y);
            
            if (position.y >= 0.f && child->Size.y)
            {
                position.y += child->Size.y;
            }
            
            if (child->Size.x >= 0.f)
            {
                childrenSize.x = glm::max(childrenSize.x, child->Size.x);
                numProcessedChildrenX++;
            } else
            {
                positionValid = false;
            }
            
            if (child->Size.y >= 0.f)
            {
                childrenSize.y += child->Size.y;
                numProcessedChildrenY++;
            } else
            {
                positionValid = false;
            }
        }
        
        if (!result)
        {
            needAnotherPass = true;
        }
    }
    
    // Make sure our minimum size encapsulates our children
    layout->MinSize.x = glm::max(glm::max(layout->MinSize.x, childrenMinSize.x), childrenSize.x);
    layout->MinSize.y = glm::max(glm::max(layout->MinSize.y, childrenMinSize.y), childrenSize.y);
    
    if (layout->Size.x < 0.f && !layout->Expand[0])
    {
        layout->Size.x = glm::max(glm::max(childrenSize.x, layout->MinSize.x), childrenMinSize.x);
    }
    
    if (layout->Size.y < 0.f && !layout->Expand[1])
    {
        layout->Size.y = glm::max(glm::max(childrenSize.y, layout->MinSize.y), childrenMinSize.y);
    }
    
    // Post-sizing pass
    for (const auto& child : layout->Children)
    {
        if (child->Size.x < 0.f)
        {
            if (child->Expand[0])
            {
                if (layout->Size.x >= 0.f)
                {
                    if (layout->LayoutType == GuiLayout::kLayoutTypeHorizontal)
                    {
                        child->Size.x = (layout->Size.x - childrenSize.x) / glm::max((float)layout->Children.size()-numProcessedChildrenX, 1.f);
                    } else if (layout->LayoutType == GuiLayout::kLayoutTypeVertical)
                    {
                        child->Size.x = layout->Size.x;
                    }
                }
            } else {
                child->Size.x = child->MinSize.x;
            }
        }
        
        if (child->Size.y < 0.f)
        {
            if (child->Expand[1])
            {
                if (layout->Size.y >= 0.f)
                {
                    if (layout->LayoutType == GuiLayout::kLayoutTypeVertical)
                    {
                        child->Size.y = (layout->Size.y - childrenSize.y) / glm::max((float)layout->Children.size()-numProcessedChildrenY, 1.f);
                    } else if (layout->LayoutType == GuiLayout::kLayoutTypeHorizontal)
                    {
                        child->Size.y = layout->Size.y;
                    }
                }
            } else {
                child->Size.y = child->MinSize.y;
            }
        }
    }
    
    layout->ContentsSize = layout->MinSize;
    
    //PbLogDebug("pb.gui", "Layout (%s) PV(%s) P(%f,%f) E(%s,%s), CS(%f,%f), S(%f,%f), MinS(%f,%f), MaxS(%f,%f) CPX(%d) CPY(%d)", needAnotherPass ? "true" : "false", layout->Tag.c_str(), layout->LocalPosition.x, layout->LocalPosition.y, layout->Expand[0] ? "true" : "false", layout->Expand[1] ? "true" : "false", childrenSize.x, childrenSize.y, layout->Size.x, layout->Size.y, layout->MinSize.x, layout->MinSize.y, layout->MaxSize.x, layout->MaxSize.y, (int)(layout->Children.size()-numProcessedChildrenX), (int)(layout->Children.size()-numProcessedChildrenY));
    
    bool invalid = needAnotherPass || !positionValid || (layout->Children.size()-numProcessedChildrenX) > 0 || (layout->Children.size()-numProcessedChildrenY) > 0 || layout->Size.x < 0.f || layout->Size.y < 0.f || layout->LocalPosition.y < 0.f || layout->LocalPosition.y < 0.f;
    
    layout->Valid = !invalid;
    
    return !invalid;
}

void GuiSystem::Clear()
{
    for (auto it = _GuiData.begin(); it != _GuiData.end();)
    {
        if (!it->second.Active)
        {
            _GuiData.erase(it++);
        } else {
            it->second.Active = false;
            ++it;
        }
    }
    
    _State.MouseWheel = glm::vec2(0,0);
    
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
        if (_State.Hot.Item)
        {
            _State.MouseDown = true;
            _State.MousePressed = true;
            return true;
        } else {
            _State.Keyboard.Item = {0,0,0};
        }
    } else if (event.Type == MouseEvent::kMouseEventUp)
    {
        _State.MouseDown = false;
        _State.MouseReleased = true;
    } else if (event.Type == MouseEvent::kMouseEventScroll)
    {
        if (_State.Hot.Item)
        {
            _State.MouseWheel += glm::vec2(inputEvent.Mouse.Scroll.Delta[0], inputEvent.Mouse.Scroll.Delta[1]);
            return true;
        }
    }
    
    return false;
}

bool GuiSystem::OnKeyboardEvent(KeyboardEvent event)
{
    GuiInputEvent inputEvent;
    inputEvent.Type = GuiInputEvent::kInputEventKeyboard;
    inputEvent.Keyboard = event;
    _InputEvents.push_back(inputEvent);
    
    if (_State.Keyboard.Item)
    {
        return true;
    }
    
    return false;
}
