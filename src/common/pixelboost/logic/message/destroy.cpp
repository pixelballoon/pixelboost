#include "pixelboost/logic/message/destroy.h"

using namespace pb;

DestroyMessage::DestroyMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

DestroyMessage::~DestroyMessage()
{
    
}

Uid DestroyMessage::GetType() const
{
    return GetStaticType();
}

Uid DestroyMessage::GetStaticType()
{
    return TypeHash("destroy");
}
