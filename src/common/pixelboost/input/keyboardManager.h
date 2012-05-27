#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/inputManager.h"

namespace pb
{
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
    
    class KeyboardHandler : public InputHandler
    {
    public:
        KeyboardHandler();
        virtual ~KeyboardHandler();
        
        virtual bool OnKeyDown(KeyboardKey key, char character);
        virtual bool OnKeyUp(KeyboardKey key, char character);
    };
    
    class KeyboardManager : public InputManager
    {
    public:
        KeyboardManager();
        ~KeyboardManager();
        
        void OnKeyDown(KeyboardKey key, char character=0);
        void OnKeyUp(KeyboardKey key, char character=0);
    };
}

#endif
