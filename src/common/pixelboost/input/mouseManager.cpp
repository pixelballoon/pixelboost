#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/mouseManager.h"

using namespace pb;

MouseHandler::MouseHandler()
{
    
}

MouseHandler::~MouseHandler()
{
    
}

bool MouseHandler::OnMouseDown(MouseButton button, glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseUp(MouseButton button, glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseMove(glm::vec2 position)
{
    return false;
}

bool MouseHandler::OnMouseScroll(glm::vec2 delta)
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

void MouseManager::OnMouseDown(MouseButton button, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseDown(button, position))
            return;
    }
}

void MouseManager::OnMouseUp(MouseButton button, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseUp(button, position))
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

void MouseManager::OnMouseScroll(glm::vec2 delta)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<MouseHandler*>(*it)->OnMouseScroll(delta))
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
