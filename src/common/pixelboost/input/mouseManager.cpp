#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/mouseManager.h"

using namespace pb;

MouseHandler::MouseHandler()
{
    
}

MouseHandler::~MouseHandler()
{
    
}

bool MouseHandler::OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseMove(glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta)
{
    return false;
}

bool MouseHandler::OnMouseZoom(glm::vec2 zoom)
{
    return false;
}

bool MouseHandler::OnMouseRotate(float rotate)
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
        if (static_cast<MouseHandler*>(*it)->OnMouseDown(button, modifierKeys, position))
            return;
    }
}

void MouseManager::OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseUp(button, modifierKeys, position))
            return;
    }
}

void MouseManager::OnMouseMove(glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseMove(position))
            return;
    }
}

void MouseManager::OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseScroll(modifierKeys, delta))
            return;
    }
}

void MouseManager::OnMouseZoom(glm::vec2 zoom)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseZoom(zoom))
            return;
    }
}

void MouseManager::OnMouseRotate(float rotate)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseRotate(rotate))
            return;
    }
}

#endif
