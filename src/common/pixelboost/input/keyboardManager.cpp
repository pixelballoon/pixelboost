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
        KeyboardEvent event;
        event.Type = KeyboardEvent::kKeyboardEventDown;
        event.Key = key;
        event.Modifier = modifier;
        event.Character = character;
        
        if (dynamic_cast<KeyboardHandler*>(*it)->OnKeyboardEvent(event))
            return;
    }
}

void KeyboardManager::OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        KeyboardEvent event;
        event.Type = KeyboardEvent::kKeyboardEventUp;
        event.Key = key;
        event.Modifier = modifier;
        event.Character = character;
        
        if (dynamic_cast<KeyboardHandler*>(*it)->OnKeyboardEvent(event))
            return;
    }
}

#endif
