#pragma once

namespace pb
{
    
    class TimelineElement
    {
    public:
        TimelineElement();
        virtual ~TimelineElement();
        
        float GetStartTime() const;
        float GetEndTime();
        
        virtual float GetLength() = 0;
        void Update(float time, bool force = false);
        void Play();
        void Stop();
        void Reset();
        
    protected:
        virtual void OnUpdate(float time, float delta) = 0;
        
        float _StartTime;
        
        bool _IsPlaying;
        float _Position;
        
        friend class Timeline;
    };

}
