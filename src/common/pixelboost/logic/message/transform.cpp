#include "pixelboost/logic/message/transform.h"

using namespace pb;

TransformChangedMessage::TransformChangedMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

TransformChangedMessage::~TransformChangedMessage()
{
    
}

Uid TransformChangedMessage::GetType() const
{
    return GetStaticType();
}

Uid TransformChangedMessage::GetStaticType()
{
    return TypeHash("transformChanged");
}
