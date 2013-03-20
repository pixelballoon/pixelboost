#pragma once

#include "pixelboost/input/inputManager.h"

namespace pb
{
    class JoystickHandler : public virtual InputHandler
    {
    public:
        JoystickHandler();
        virtual ~JoystickHandler();
        
        virtual bool OnAxisChanged(int joystick, int stick, int axis, float value);
        virtual bool OnButtonDown(int joystick, int button);
        virtual bool OnButtonUp(int joystick, int button);
    };
    
    class JoystickManager : public InputManager
    {
    public:
        JoystickManager();
        ~JoystickManager();
        
        void OnAxisChanged(int joystick, int stick, int axis, float value);
        void OnButtonDown(int joystick, int button);
        void OnButtonUp(int joystick, int button);
    };
}
