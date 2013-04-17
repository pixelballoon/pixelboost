#include <functional>

#include "pixelboost/animation/timeline/element.h"

namespace pb
{
    class TimelineFunction : public TimelineElement
    {
    public:
        TimelineFunction(std::function<void(void)> function);
        ~TimelineFunction();
        
        virtual float GetLength();
        
    private:
        virtual void OnPlay();
        
        std::function<void(void)> _Function;
    };
}
