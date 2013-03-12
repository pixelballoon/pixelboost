#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/inputManager.h"

namespace pb
{
    typedef int ModifierKeys;
    
    enum KeyboardKey
    {
        kKeyboardKeyCharacter,
        kKeyboardKeyReturn,
        kKeyboardKeyBackspace,
        kKeyboardKeyDelete,
        kKeyboardKeyLeft,
        kKeyboardKeyRight,
        kKeyboardKeyShift,
        kKeyboardKeyTab,
        kKeyboardKeySpace,
        kKeyboardKeyHome,
        kKeyboardKeyEnd,
        kKeyboardKeyControl,
        kKeyboardKeyUp,
        kKeyboardKeyDown,
        kKeyboardKeyEscape,
        kKeyboardKeyAlt
    };
    
    enum ModifierKey
    {
        kModifierKeyNone = 0,
        kModifierKeyControl = 1,
        kModifierKeyShift = 2,
        kModifierKeyAlt = 4,
    };
    
    struct KeyboardEvent
    {
        enum EventType
        {
            kKeyboardEventDown,
            kKeyboardEventUp,
        } Type;
        
        KeyboardKey Key;
        ModifierKeys Modifier;
        char Character;
    };
    
    class KeyboardHandler : public virtual InputHandler
    {
    public:
        KeyboardHandler();
        virtual ~KeyboardHandler();
        
        virtual bool OnKeyboardEvent(KeyboardEvent event) = 0;
    };
    
    class KeyboardManager : public InputManager
    {
    public:
        KeyboardManager();
        ~KeyboardManager();
        
        void OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character=0);
        void OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character=0);
    };
}

#endif
