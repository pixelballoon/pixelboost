#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include <vector>

#include "pixelboost/input/inputManager.h"

namespace pb
{
    class InputHandler
    {
    public:
        InputHandler();
        virtual ~InputHandler();
        
        virtual int GetPriority();
        
    private:
        bool _Enabled;
        
        friend class InputManager;
    };
    
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();
        
        void AddHandler(InputHandler* handler);
        void RemoveHandler(InputHandler* handler);
        
    protected:
        void UpdateHandlers();
        
        typedef std::vector<InputHandler*> HandlerList;
        
        HandlerList _Handlers;
        HandlerList _HandlersToAdd;
    };
    
}

#endif
