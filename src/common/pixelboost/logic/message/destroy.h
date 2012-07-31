#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class DestroyMessage : public Message
{
public:
    DestroyMessage(Entity* entity, Component* component);
    virtual ~DestroyMessage();
    
    virtual Uid GetType() const;
    static Uid GetStaticType();    
};
    
}
