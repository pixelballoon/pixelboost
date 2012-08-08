#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{

class Viewport;
    
class TouchMessage : public Message
{
public:
    enum TouchType
    {
        kTouchTypeDown,
        kTouchTypeMove,
        kTouchTypeUp,
    };
    
    TouchMessage(Entity* entity, Component* component, TouchType touchType, glm::vec2 touchPosition);
    virtual ~TouchMessage();
    
    Uid GetType() const;
    static Uid GetStaticType();
    
public:
    TouchType GetTouchType();
    
    glm::vec2 GetTouchPosition();
    
private:
    TouchType _TouchType;
    
    glm::vec2 _TouchPosition;
};
    
}
