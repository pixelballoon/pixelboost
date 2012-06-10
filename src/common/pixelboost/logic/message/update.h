#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class UpdateMessage : public Message
{
public:
    UpdateMessage(float delta);
    virtual ~UpdateMessage();
    
    Uid GetType() const;
    static Uid GetStaticType();
    
    float GetDelta() const;
    
private:
    float _Delta;
};
    
}
