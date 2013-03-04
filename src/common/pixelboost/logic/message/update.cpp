#include "pixelboost/logic/message/update.h"

using namespace pb;

PB_DEFINE_MESSAGE(UpdateMessage)

UpdateMessage::UpdateMessage(float timeDelta, float gameDelta)
    : Message(0, 0)
    , _GameDelta(gameDelta)
    , _TimeDelta(timeDelta)
{
    
}

UpdateMessage::~UpdateMessage()
{
    
}

float UpdateMessage::GetGameDelta() const
{
    return _GameDelta;
}

float UpdateMessage::GetTimeDelta() const
{
    return _TimeDelta;
}
