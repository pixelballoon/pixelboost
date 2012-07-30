#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class Viewport;
    
class TouchMessage : public Message
{
public:
    TouchMessage(Entity* entity, Component* component);
    virtual ~TouchMessage();
    
    Uid GetType() const;
    static Uid GetStaticType();
};
    
}
