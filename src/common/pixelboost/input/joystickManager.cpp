#include "pixelboost/input/joystickManager.h"

using namespace pb;

JoystickHandler::JoystickHandler()
{
    
}

JoystickHandler::~JoystickHandler()
{
    
}

bool JoystickHandler::OnAxisChanged(int joystick, int stick, int axis, float value)
{
    return false;
}

bool JoystickHandler::OnButtonDown(int joystick, int button)
{
    return false;
}

bool JoystickHandler::OnButtonUp(int joystick, int button)
{
    return false;
}

JoystickManager::JoystickManager()
{
    
}

JoystickManager::~JoystickManager()
{
    
}

void JoystickManager::OnAxisChanged(int joystick, int stick, int axis, float value)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (dynamic_cast<JoystickHandler*>(*it)->OnAxisChanged(joystick, stick, axis, value))
            return;
    }
}

void JoystickManager::OnButtonDown(int joystick, int button)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (dynamic_cast<JoystickHandler*>(*it)->OnButtonDown(joystick, button))
            return;
    }
}

void JoystickManager::OnButtonUp(int joystick, int button)
{
    UpdateHandlers();
    
    for (HandlerList::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
    {
        if (dynamic_cast<JoystickHandler*>(*it)->OnButtonUp(joystick, button))
            return;
    }
}
