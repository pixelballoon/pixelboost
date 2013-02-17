#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include "glm/glm.hpp"

#include "pixelboost/input/inputManager.h"

namespace pb
{
    class Viewport;
    
    class Touch
    {
    public:
        Touch();
        Touch(int touchId, Viewport* viewport, glm::vec2 position);
              
        bool IsValid();
        int GetId();
        pb::Viewport* GetViewport();
        glm::vec2 GetScreenPosition();
        glm::vec2 GetViewportPosition();
        
        bool operator==(Touch const& b);
        bool operator!=(Touch const& b);
        
    private:
        bool _IsValid;
        int _Id;
        Viewport* _Viewport;
        glm::vec2 _Position;
    };
    
    class TouchHandler : public virtual InputHandler
    {
    public:
        TouchHandler();
        virtual ~TouchHandler();
        
        virtual bool OnTouchDown(Touch touch);
        virtual bool OnTouchUp(Touch touch);
        virtual bool OnTouchMove(Touch touch);
    };
    
    class TouchManager : public InputManager
    {
    public:
        TouchManager();
        ~TouchManager();
        
        void OnTouchDown(int touchId, glm::vec2 position);
        void OnTouchUp(int touchId, glm::vec2 position);
        void OnTouchMove(int touchId, glm::vec2 position);
    };
    
}

#endif
