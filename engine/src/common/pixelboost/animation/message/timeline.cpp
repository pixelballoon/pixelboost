#include "pixelboost/animation/message/timeline.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::TimelinePlayingMessage)

TimelinePlayingMessage::TimelinePlayingMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

PB_DEFINE_MESSAGE(pb::TimelineStoppedMessage)

TimelineStoppedMessage::TimelineStoppedMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}
