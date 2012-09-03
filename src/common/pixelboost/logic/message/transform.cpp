#include "pixelboost/logic/message/transform.h"

namespace pb
{

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
    return TypeHash("pb::TransformChangedMessage");
}

}
