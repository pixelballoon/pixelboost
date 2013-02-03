#include "pixelboost/logic/message/update.h"

using namespace pb;

UpdateMessage::UpdateMessage(float timeDelta, float gameDelta)
    : Message(0, 0)
    , _GameDelta(gameDelta)
    , _TimeDelta(timeDelta)
{
    
}

UpdateMessage::~UpdateMessage()
{
    
}

Uid UpdateMessage::GetType() const
{
    return GetStaticType();
}

Uid UpdateMessage::GetStaticType()
{
    return TypeHash("pb::UpdateMessage");
}

float UpdateMessage::GetGameDelta() const
{
    return _GameDelta;
}

float UpdateMessage::GetTimeDelta() const
{
    return _TimeDelta;
}
