#include "pixelboost/animation/timeline/function.h"

using namespace pb;

TimelineFunction::TimelineFunction(std::function<void(void)> function)
{
    _Function = function;
}

TimelineFunction::~TimelineFunction()
{
    
}

float TimelineFunction::GetLength()
{
    return 0.f;
}

void TimelineFunction::OnPlay()
{
    _Function();
}
