#include "pixelboost/logic/message/input/touch.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::TouchMessage)

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

PB_DEFINE_MESSAGE(pb::TouchDownMessage)

TouchDownMessage::TouchDownMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchDownMessage::~TouchDownMessage()
{
    
}

PB_DEFINE_MESSAGE(pb::TouchMoveMessage)

TouchMoveMessage::TouchMoveMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchMoveMessage::~TouchMoveMessage()
{
    
}

PB_DEFINE_MESSAGE(pb::TouchUpMessage)

TouchUpMessage::TouchUpMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchUpMessage::~TouchUpMessage()
{
    
}

PB_DEFINE_MESSAGE(pb::TouchPressedMessage)

TouchPressedMessage::TouchPressedMessage(Entity* entity, Component* component, int touchIndex, glm::vec2 touchPosition)
    : TouchMessage(entity, component, touchIndex, touchPosition)
{
    
}

TouchPressedMessage::~TouchPressedMessage()
{
    
}
