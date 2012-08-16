#include "pixelboost/logic/message/input/touch.h"

using namespace pb;

TouchMessage::TouchMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : Message(entity, component)
    , _TouchIndex(touchIndex)
    , _TouchPosition(touchPosition)
{
    
}

TouchMessage::~TouchMessage()
{
    
}

int TouchMessage::GetTouchIndex() const
{
    return _TouchIndex;
}

glm::vec2 TouchMessage::GetTouchPosition() const
{
    return _TouchPosition;
}

TouchDownMessage::TouchDownMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchDownMessage::~TouchDownMessage()
{
    
}

Uid TouchDownMessage::GetType() const
{
    return GetStaticType();
}

Uid TouchDownMessage::GetStaticType()
{
    return TypeHash("pb::TouchDownMessage");
}

TouchMoveMessage::TouchMoveMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchMoveMessage::~TouchMoveMessage()
{
    
}

Uid TouchMoveMessage::GetType() const
{
    return GetStaticType();
}

Uid TouchMoveMessage::GetStaticType()
{
    return TypeHash("pb::TouchMoveMessage");
}

TouchUpMessage::TouchUpMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchUpMessage::~TouchUpMessage()
{
    
}

Uid TouchUpMessage::GetType() const
{
    return GetStaticType();
}

Uid TouchUpMessage::GetStaticType()
{
    return TypeHash("pb::TouchUpMessage");
}
