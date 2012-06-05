#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class UpdateMessage : public Message
{
public:
    UpdateMessage(float delta);
    virtual ~UpdateMessage();
    
    Uid GetType();
    static Uid GetStaticType();
    
    float GetDelta();
    
private:
    float _Delta;
};
    
}
