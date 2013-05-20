#include <algorithm>

#include "pixelboost/animation/timeline/timeline.h"
#include "pixelboost/debug/log.h"

using namespace pb;

Timeline::Timeline()
{
    _LengthDirty = true;
}

Timeline::~Timeline()
{
    for (const auto& element : _Elements)
    {
        delete element;
    }
}

float Timeline::GetLength()
{
    if (_LengthDirty)
    {
        _Length = 0.f;
        
        if (_Elements.size())
        {
            for (auto element : _Elements)
            {
                _Length = glm::max(_Length, element->GetEndTime());
            }
        }
    }
    
    return _Length;
}

void Timeline::Insert(TimelineElement* element, float time)
{
    auto bound = std::lower_bound(_Elements.begin(), _Elements.end(), time, [](const TimelineElement* element, float time) {return element->GetStartTime() < time;} );
    auto item = _Elements.insert(bound, element);
    (*item)->_StartTime = time;
}

void Timeline::Append(TimelineElement* element)
{
    Insert(element, GetLength());
}

void Timeline::AddLabel(const std::string& name, float time)
{
    _Labels[name] = time;
}

void Timeline::OnUpdate(float time, float delta)
{
    auto start = std::lower_bound(_Elements.begin(), _Elements.end(), time - delta, [](const TimelineElement* element, float time) {return element->GetStartTime() < time;} );
    auto end = std::upper_bound(_Elements.begin(), _Elements.end(), time, [](float time, const TimelineElement* element) {return time <= element->GetStartTime();} );
    
    auto it = start;
    
    while (it != end)
    {
        (*it)->Reset();
        (*it)->Play();
        it++;
    }
    
    for (auto element : _Elements)
    {
        element->Update(delta);
    }
}
