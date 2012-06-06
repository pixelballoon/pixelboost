#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{
    
class TransformChangedMessage : public Message
{
public:
    TransformChangedMessage(Entity* entity, Component* component);
    virtual ~TransformChangedMessage();
    
    Uid GetType();
    static Uid GetStaticType();
    
private:
};
    
}
