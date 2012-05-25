#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/keyboardManager.h"

using namespace pb;

KeyboardHandler::KeyboardHandler()
{
    
}

KeyboardHandler::~KeyboardHandler()
{
    
}

bool KeyboardHandler::OnKeyDown(KeyboardKey key, char character)
{
    return false;
}

bool KeyboardHandler::OnKeyUp(KeyboardKey key, char character)
{
    return false;
}

KeyboardManager::KeyboardManager()
{
    
}

KeyboardManager::~KeyboardManager()
{
    
}

void KeyboardManager::OnKeyDown(KeyboardKey key, char character)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<KeyboardHandler*>(*it)->OnKeyDown(key, character))
            return;
    }
}

void KeyboardManager::OnKeyUp(KeyboardKey key, char character)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (static_cast<KeyboardHandler*>(*it)->OnKeyUp(key, character))
            return;
    }
}

#endif
