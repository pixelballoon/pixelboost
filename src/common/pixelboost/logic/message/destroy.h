#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class DestroyMessage : public Message
{
    PB_DECLARE_MESSAGE
    
public:
    DestroyMessage(Entity* entity, Component* component);
    virtual ~DestroyMessage();
};
    
}
