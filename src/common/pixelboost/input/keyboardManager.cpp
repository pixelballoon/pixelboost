#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/keyboardManager.h"

using namespace pb;

KeyboardHandler::KeyboardHandler()
{
    
}

KeyboardHandler::~KeyboardHandler()
{
    
}

bool KeyboardHandler::OnKeyDown(int key)
{
    return false;
}

bool KeyboardHandler::OnKeyUp(int key)
{
    return false;
}

KeyboardManager::KeyboardManager()
{
    
}

KeyboardManager::~KeyboardManager()
{
    
}

void KeyboardManager::OnKeyDown(int key)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<KeyboardHandler*>(*it)->OnKeyDown(key))
            return;
    }
}

void KeyboardManager::OnKeyUp(int key)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<KeyboardHandler*>(*it)->OnKeyUp(key))
            return;
    }
}

#endif
