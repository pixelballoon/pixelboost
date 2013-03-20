#pragma once

#include "glm/glm.hpp"

#include "pixelboost/input/inputManager.h"
#include "pixelboost/input/keyboardManager.h"

namespace pb
{
    class Viewport;
    
    enum MouseModifier
    {
        kModifierShift = 1,
        kModifierCtrl = 2,
        kModifierAlt = 4,
    };
    
    enum MouseButton
    {
        kMouseButtonLeft = 0,
        kMouseButtonMiddle = 1,
        kMouseButtonRight = 2
    };
    
    struct MouseEvent
    {
        enum EventType
        {
            kMouseEventDown,
            kMouseEventUp,
            kMouseEventMove,
            kMouseEventScroll,
            kMouseEventRotate,
            kMouseEventZoom,
        } Type;
        
        Viewport* Viewport;
        
        struct ButtonEvent
        {
            MouseButton Button;
            ModifierKeys ModifierKeys;
            float Position[2];
        };
        
        struct MoveEvent
        {
            float Position[2];
        };
        
        struct ScrollEvent
        {
            ModifierKeys Modifier;
            float Delta[2];
        };
        
        struct ZoomEvent
        {
            float Delta[2];
        };
        
        struct RotateEvent
        {
            float Value;
        };
        
        union
        {
            ButtonEvent Down;
            ButtonEvent Up;
            MoveEvent Move;
            ScrollEvent Scroll;
            ZoomEvent Zoom;
            RotateEvent Rotate;
        };
    };
    
    class MouseHandler : public virtual InputHandler
    {
    public:
        MouseHandler();
        virtual ~MouseHandler();
        
        virtual bool OnMouseEvent(MouseEvent event) = 0;
    };
    
    class MouseManager : public InputManager
    {
    public:
        MouseManager();
        ~MouseManager();
        
        void OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        void OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        void OnMouseMove(glm::vec2 position);
        void OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta);
        void OnMouseZoom(glm::vec2 zoom);
        void OnMouseRotate(float rotate);
    };
    
}
