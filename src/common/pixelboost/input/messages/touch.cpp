#include "pixelboost/input/messages/touch.h"

using namespace pb;

TouchMessage::TouchMessage(Entity* entity, Component* component)
    : Message(entity, component)
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
