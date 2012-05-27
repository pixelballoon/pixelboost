#ifndef PIXELBOOST_DISABLE_INPUT

#include <algorithm>

#include "pixelboost/input/inputManager.h"

using namespace pb;

static bool HandlerSort (InputHandler* a, InputHandler* b)
{
    return a->GetPriority() > b->GetPriority();
}

InputHandler::InputHandler()
{
    
}

InputHandler::~InputHandler()
{
    
}

int InputHandler::GetPriority()
{
    return -1;
}

InputManager::InputManager()
{
    
}

InputManager::~InputManager()
{
    
}

void InputManager::AddHandler(InputHandler* handler)
{
    handler->_Enabled = true;
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (*it == handler)
            return;
    }
    
    for (HandlerList::iterator it = _HandlersToAdd.begin(); it != _HandlersToAdd.end(); ++it)
    {
        if (*it == handler)
            return;
    }
    
    _HandlersToAdd.push_back(handler);
}

void InputManager::RemoveHandler(InputHandler* handler)
{
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (*it == handler)
        {
            handler->_Enabled = false;
        }
    }
    
    for (HandlerList::iterator it = _HandlersToAdd.begin(); it != _HandlersToAdd.end(); ++it)
    {
        if (*it == handler)
        {
            handler->_Enabled = false;
        }
    }
}

void InputManager::UpdateHandlers()
{
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end();)
    {
        if ((*it)->_Enabled == false)
        {
            it = _Handlers.erase(it);
        } else {
            ++it;
        }
    }
    
    for (HandlerList::iterator it = _HandlersToAdd.begin(); it != _HandlersToAdd.end(); ++it)
    {
        if ((*it)->_Enabled == true)
        {
            _Handlers.push_back(*it);
        }
    }
    _HandlersToAdd.clear();
    
    std::sort(_Handlers.begin(), _Handlers.end(), &HandlerSort);
}

#endif
