#include "pixelboost/logic/message/update.h"

using namespace pb;

UpdateMessage::UpdateMessage(Uid source, float delta)
    : Message(source)
    , _Delta(delta)
{
    
}

UpdateMessage::~UpdateMessage()
{
    
}

Uid UpdateMessage::GetType()
{
    return TypeHash("update");
}

float UpdateMessage::GetDelta()
{
    return _Delta;
}
