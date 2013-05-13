#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{

class Viewport;
    
    class TouchMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        TouchMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchMessage();
        
    public:
        int GetTouchIndex() const;
        glm::vec2 GetTouchPosition() const;
        
    private:
        int _TouchIndex;
        glm::vec2 _TouchPosition;
    };
    
    class TouchDownMessage : public TouchMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        TouchDownMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchDownMessage();
    };
    
    class TouchMoveMessage : public TouchMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        TouchMoveMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchMoveMessage();
    };
    
    class TouchUpMessage : public TouchMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        TouchUpMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchUpMessage();
    };
    
    class TouchPressedMessage : public TouchMessage
    {
        PB_DECLARE_MESSAGE
        
    public:
        TouchPressedMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchPressedMessage();
    };
    
}
