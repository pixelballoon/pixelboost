#pragma once

#ifndef PIXELBOOST_DISABLE_INPUT

#include "glm/glm.hpp"

#include "pixelboost/input/inputManager.h"

namespace pb
{
    enum MouseButton
    {
        kMouseButtonLeft,
        kMouseButtonMiddle,
        kMouseButtonRight
    };
    
    class MouseHandler : public InputHandler
    {
    public:
        MouseHandler();
        virtual ~MouseHandler();
        
        virtual bool OnMouseDown(MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnMouseScroll(glm::vec2 delta);
        virtual bool OnMouseZoom(glm::vec2 zoom);
        virtual bool OnMouseRotate(float rotate);
    };
    
    class MouseManager : public InputManager
    {
    public:
        MouseManager();
        ~MouseManager();
        
        void OnMouseDown(MouseButton button, glm::vec2 position);
        void OnMouseUp(MouseButton button, glm::vec2 position);
        void OnMouseMove(glm::vec2 position);
        void OnMouseScroll(glm::vec2 delta);
        void OnMouseZoom(glm::vec2 zoom);
        void OnMouseRotate(float rotate);
    };
    
}

#endif
