#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;
    class Timeline;
    
    class TimelineComponent : public pb::Component
    {
        PB_DECLARE_COMPONENT
        
    public:
        TimelineComponent(Entity* parent);
        virtual ~TimelineComponent();
        
        Timeline* GetTimeline();
        void Play();

    private:
        void OnUpdate(const Message& message);
        
        Timeline* _Timeline;
    };

}
