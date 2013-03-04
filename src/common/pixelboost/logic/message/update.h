#pragma once

#include "pixelboost/logic/message.h"

namespace pb
{

class UpdateMessage : public Message
{
    PB_DECLARE_MESSAGE
    
public:
    UpdateMessage(float timeDelta, float gameDelta);
    virtual ~UpdateMessage();
    
    float GetGameDelta() const;
    float GetTimeDelta() const;
    
private:
    float _GameDelta;
    float _TimeDelta;
};
    
}
