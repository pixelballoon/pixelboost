#include "pixelboost/logic/message/transform.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::TransformChangedMessage)

TransformChangedMessage::TransformChangedMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

TransformChangedMessage::~TransformChangedMessage()
{
    
}
