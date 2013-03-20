#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class ParticleFinishedMessage : public Message
{
    PB_DECLARE_MESSAGE
    
public:
    ParticleFinishedMessage(Entity* entity, Component* component);
    virtual ~ParticleFinishedMessage();
    
private:
};
    
}
