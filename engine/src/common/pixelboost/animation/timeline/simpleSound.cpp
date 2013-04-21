#include "pixelboost/animation/timeline/simpleSound.h"
#include "pixelboost/audio/audioManagerSimple.h"
#include "pixelboost/debug/log.h"

using namespace pb;

TimelineSimpleSound::TimelineSimpleSound(const std::string& sound, float volume, float pitch)
{
    _Sound = sound;
    _Volume = volume;
    _Pitch = pitch;
}

TimelineSimpleSound::~TimelineSimpleSound()
{
    
}

float TimelineSimpleSound::GetLength()
{
    return 0.f;
}

void TimelineSimpleSound::OnPlay()
{
    pb::AudioManagerSimple::Instance()->PlaySfx(_Sound, _Volume, _Pitch);
}
