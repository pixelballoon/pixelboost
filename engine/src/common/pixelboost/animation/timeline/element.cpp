#include <algorithm>

#include "pixelboost/animation/timeline/timeline.h"

using namespace pb;

TimelineElement::TimelineElement()
{
    _StartTime = 0.f;
    
    _IsPlaying = false;
    _Position = 0.f;
}

TimelineElement::~TimelineElement()
{
    
}

float TimelineElement::GetStartTime() const
{
    return _StartTime;
}

float TimelineElement::GetEndTime()
{
    return _StartTime + GetLength();
}

bool TimelineElement::IsPlaying() const
{
    return _IsPlaying;
}

void TimelineElement::Update(float delta, bool force)
{
    if ((_IsPlaying || force) && GetLength() > 0.f)
    {
        _Position = glm::min(_Position+delta, GetLength());
        
        OnUpdate(_Position, delta);
        
        if (_Position == GetLength())
        {
            _IsPlaying = false;
        }
    }
}

void TimelineElement::Play()
{
    _IsPlaying = true;
    OnPlay();
    Update(0, true);
}

void TimelineElement::Stop()
{
    _IsPlaying = false;
    OnStop();
}

void TimelineElement::Reset()
{
    _Position = 0.f;
    Update(0, true);
}

void TimelineElement::OnPlay()
{

}

void TimelineElement::OnStop()
{
    
}

void TimelineElement::OnUpdate(float time, float delta)
{

}
