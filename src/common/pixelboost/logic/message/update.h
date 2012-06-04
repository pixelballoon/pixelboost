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
    static Uid GetStaticType();
    
    float GetDelta();
    
private:
    float _Delta;
};
    
}
