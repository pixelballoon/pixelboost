#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/input/mouseManager.h"

using namespace pb;

MouseHandler::MouseHandler()
{
    
}

MouseHandler::~MouseHandler()
{
    
}

bool MouseHandler::OnMouseEvent(MouseEvent event)
{
    return false;
}

MouseManager::MouseManager()
{
    
}

MouseManager::~MouseManager()
{

}

void MouseManager::OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventDown;
            event.Viewport = viewport;
            event.Down.Button = button;
            event.Down.ModifierKeys = modifierKeys;
            event.Down.Position[0] = position.x;
            event.Down.Position[1] = position.y;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}

void MouseManager::OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventUp;
            event.Viewport = viewport;
            event.Up.Button = button;
            event.Up.ModifierKeys = modifierKeys;
            event.Up.Position[0] = position.x;
            event.Up.Position[1] = position.y;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}

void MouseManager::OnMouseMove(glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventMove;
            event.Viewport = viewport;
            event.Move.Position[0] = position.x;
            event.Move.Position[1] = position.y;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}

void MouseManager::OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta)
{
    UpdateHandlers();
        
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventScroll;
            event.Viewport = viewport;
            event.Scroll.Delta[0] = delta.x;
            event.Scroll.Delta[1] = delta.y;
            event.Scroll.Modifier = modifierKeys;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}

void MouseManager::OnMouseZoom(glm::vec2 zoom)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventZoom;
            event.Viewport = viewport;
            event.Zoom.Delta[0] = zoom.x;
            event.Zoom.Delta[1] = zoom.y;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}

void MouseManager::OnMouseRotate(float rotate)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        for (auto viewport : Renderer::Instance()->GetViewports())
        {
            MouseEvent event;
            event.Type = MouseEvent::kMouseEventRotate;
            event.Viewport = viewport;
            event.Rotate.Value = rotate;
            
            if (dynamic_cast<MouseHandler*>(*it)->OnMouseEvent(event))
                return;
        }
    }
}
