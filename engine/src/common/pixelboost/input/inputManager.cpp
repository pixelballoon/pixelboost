#include <algorithm>

#include "pixelboost/input/inputManager.h"

using namespace pb;

static bool HandlerSort (InputHandler* a, InputHandler* b)
{
    return a->GetInputHandlerPriority() > b->GetInputHandlerPriority();
}

InputHandler::~InputHandler()
{
    
}

InputManager::InputManager()
{
    
}

InputManager::~InputManager()
{
    
}

void InputManager::AddHandler(InputHandler* handler)
{
    HandlerMap::iterator it = _HandlerMap.find(handler);
    if (it != _HandlerMap.end())
    {
        it->second = true;
        return;
    }
    
    _HandlersToAdd[handler] = true;
}

void InputManager::RemoveHandler(InputHandler* handler)
{
    HandlerMap::iterator it = _HandlerMap.find(handler);
    if (it != _HandlerMap.end())
    {
        it->second = false;
        return;
    }
    
    it = _HandlersToAdd.find(handler);
    if (it != _HandlersToAdd.end())
    {
        it->second = false;
        return;
    }    
}

void InputManager::UpdateHandlers()
{
    for (HandlerMap::iterator it = _HandlerMap.begin(); it != _HandlerMap.end();)
    {
        if (it->second == false)
        {
            _HandlerMap.erase(it++);
        } else {
            ++it;
        }
    }
    
    for (HandlerMap::iterator it = _HandlersToAdd.begin(); it != _HandlersToAdd.end(); ++it)
    {
        if (it->second == true)
        {
            _HandlerMap[it->first] = true;
        }
    }
    _HandlersToAdd.clear();
    
    _Handlers.clear();
    for (HandlerMap::iterator it = _HandlerMap.begin(); it != _HandlerMap.end(); ++it)
    {
        _Handlers.push_back(it->first);
    }
    
    std::sort(_Handlers.begin(), _Handlers.end(), &HandlerSort);
}
