#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/keyboardManager.h"

using namespace pb;

KeyboardHandler::KeyboardHandler()
{
    
}

KeyboardHandler::~KeyboardHandler()
{
    
}

KeyboardManager::KeyboardManager()
{
    
}

KeyboardManager::~KeyboardManager()
{
    
}

void KeyboardManager::OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (dynamic_cast<KeyboardHandler*>(*it)->OnKeyDown(key, modifier, character))
            return;
    }
}

void KeyboardManager::OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (dynamic_cast<KeyboardHandler*>(*it)->OnKeyUp(key, modifier, character))
            return;
    }
}

#endif
