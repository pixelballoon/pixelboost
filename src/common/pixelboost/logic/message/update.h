#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class UpdateMessage : public Message
{
public:
    UpdateMessage(float timeDelta, float gameDelta);
    virtual ~UpdateMessage();
    
    Uid GetType() const;
    static Uid GetStaticType();
    
    float GetGameDelta() const;
    float GetTimeDelta() const;
    
private:
    float _GameDelta;
    float _TimeDelta;
};
    
}
