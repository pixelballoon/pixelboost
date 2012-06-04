#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{

class Message
{
public:
    Message(Uid source);
    virtual ~Message();
    
    virtual Uid GetType() = 0;
    
private:
    Uid _Source;
};
    
}
