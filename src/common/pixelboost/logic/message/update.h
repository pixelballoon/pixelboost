#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class UpdateMessage : public Message
{
public:
    UpdateMessage(Uid source, float delta);
    virtual ~UpdateMessage();
    
    Uid GetType();
    
    float GetDelta();
    
private:
    float _Delta;
};
    
}
