#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/input/inputManager.h"

namespace pb
{
    class KeyboardHandler : public InputHandler
    {
    public:
        KeyboardHandler();
        virtual ~KeyboardHandler();
        
        virtual bool OnKeyDown(int key);
        virtual bool OnKeyUp(int key);
    };
    
    class KeyboardManager : public InputManager
    {
    public:
        KeyboardManager();
        ~KeyboardManager();
        
        void OnKeyDown(int key);
        void OnKeyUp(int key);
    };
}

#endif
