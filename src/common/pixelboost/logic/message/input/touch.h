#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{

class Viewport;
    
    class TouchMessage : public Message
    {
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
    public:
        TouchDownMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchDownMessage();
        
        Uid GetType() const;
        static Uid GetStaticType();
    };
    
    class TouchMoveMessage : public TouchMessage
    {
    public:
        TouchMoveMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchMoveMessage();
        
        Uid GetType() const;
        static Uid GetStaticType();
    };
    
    class TouchUpMessage : public TouchMessage
    {
    public:
        TouchUpMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition);
        virtual ~TouchUpMessage();
        
        Uid GetType() const;
        static Uid GetStaticType();
    };
    
}
