#include "pixelboost/logic/message/input/touch.h"

using namespace pb;

TouchMessage::TouchMessage(Entity* entity, Component* component, int touchIndex, TouchType touchType, glm::vec2 touchPosition)
    : Message(entity, component)
    , _TouchIndex(touchIndex)
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
    return TypeHash("pb::TouchMessage");
}

int TouchMessage::GetTouchIndex() const
{
    return _TouchIndex;
}

TouchMessage::TouchType TouchMessage::GetTouchType() const
{
    return _TouchType;
}

glm::vec2 TouchMessage::GetTouchPosition() const
{
    return _TouchPosition;
}
