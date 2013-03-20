#include "pixelboost/logic/message/destroy.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::DestroyMessage)

DestroyMessage::DestroyMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

DestroyMessage::~DestroyMessage()
{
    
}
