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
    Update(0, true);
}

void TimelineElement::Stop()
{
    _IsPlaying = false;
}

void TimelineElement::Reset()
{
    _Position = 0.f;
    Update(0, true);
}