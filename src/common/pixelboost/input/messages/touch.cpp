#include "pixelboost/input/messages/touch.h"

using namespace pb;

TouchMessage::TouchMessage(Entity* entity, Component* component, TouchType touchType, glm::vec2 touchPosition)
    : Message(entity, component)
    , _TouchType(touchType)
    , _TouchPosition(touchPosition)
{
    
}

TouchMessage::~TouchMessage()
{
    
}

Uid TouchMessage::GetType() const
{
    return GetStaticType();
}

Uid TouchMessage::GetStaticType()
{
    return TypeHash("touch");
}

TouchMessage::TouchType TouchMessage::GetTouchType()
{
    return _TouchType;
}

glm::vec2 TouchMessage::GetTouchPosition()
{
    return _TouchPosition;
}
