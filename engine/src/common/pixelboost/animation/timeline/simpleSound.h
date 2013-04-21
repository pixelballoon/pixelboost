#include <string>

#include "pixelboost/animation/timeline/element.h"

namespace pb
{
    class TimelineSimpleSound : public TimelineElement
    {
    public:
        TimelineSimpleSound(const std::string& sound, float volume = 1.f, float pitch = 1.f);
        ~TimelineSimpleSound();
        
        virtual float GetLength();
        
    private:
        virtual void OnPlay();
        
        std::string _Sound;
        float _Volume;
        float _Pitch;
    };
}
