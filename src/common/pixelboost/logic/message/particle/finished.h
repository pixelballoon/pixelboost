#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class ParticleFinishedMessage : public Message
{
public:
    ParticleFinishedMessage(Entity* entity, Component* component);
    virtual ~ParticleFinishedMessage();
    
    Uid GetType() const;
    static Uid GetStaticType();
    
private:
};
    
}
