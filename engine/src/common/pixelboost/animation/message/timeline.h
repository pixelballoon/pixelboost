#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class TimelinePlayingMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        TimelinePlayingMessage(Entity* entity, Component* component);
    };
    
    class TimelineStoppedMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        TimelineStoppedMessage(Entity* entity, Component* component);
    };
    
}
