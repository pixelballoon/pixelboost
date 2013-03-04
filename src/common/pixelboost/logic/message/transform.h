#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class TransformChangedMessage : public Message
{
    PB_DECLARE_MESSAGE
    
public:
    TransformChangedMessage(Entity* entity, Component* component);
    virtual ~TransformChangedMessage();
    
private:
};
    
}
