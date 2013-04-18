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
        
        bool IsPlaying() const;
        
        virtual float GetLength() = 0;
        void Update(float time, bool force = false);
        void Play();
        void Stop();
        void Reset();
        
    protected:
        virtual void OnPlay();
        virtual void OnStop();
        virtual void OnUpdate(float time, float delta);
        
        float _StartTime;
        
        bool _IsPlaying;
        float _Position;
        
        friend class Timeline;
    };

}
