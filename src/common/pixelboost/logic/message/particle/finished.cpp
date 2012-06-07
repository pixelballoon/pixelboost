#include "pixelboost/logic/message/particle/finished.h"

using namespace pb;

ParticleFinishedMessage::ParticleFinishedMessage(Entity* entity, Component* component)
: Message(entity, component)
{
    
}

ParticleFinishedMessage::~ParticleFinishedMessage()
{
    
}

Uid ParticleFinishedMessage::GetType()
{
    return GetStaticType();
}

Uid ParticleFinishedMessage::GetStaticType()
{
    return TypeHash("particleFinished");
}
