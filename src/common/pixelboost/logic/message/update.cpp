#include "pixelboost/logic/message/update.h"

using namespace pb;

UpdateMessage::UpdateMessage(float delta)
    : Message(0, 0)
    , _Delta(delta)
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

float UpdateMessage::GetDelta() const
{
    return _Delta;
}
