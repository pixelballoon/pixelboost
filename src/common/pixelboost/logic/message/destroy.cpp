#include "pixelboost/logic/message/destroy.h"

using namespace pb;

PB_DEFINE_MESSAGE(DestroyMessage)

DestroyMessage::DestroyMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

DestroyMessage::~DestroyMessage()
{
    
}
