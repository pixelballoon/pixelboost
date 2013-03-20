#include "pixelboost/audio/audioManagerSimple.h"

using namespace pb;

AudioManagerSimple::Sound::Sound(int id, const std::string& name, float volume, float pitch, bool looping)
    : _Id(id)
    , _Looping(false)
    , _Name(name)
    , _IsPlaying(false)
    , _Pitch(pitch)
    , _Volume(volume)
{
    
}

int AudioManagerSimple::Sound::GetId() const
{
    return _Id;
}

void AudioManagerSimple::Sound::Play()
{
    _IsPlaying = true;
    AudioManagerSimple::Instance()->SfxPlay(*this);
}

void AudioManagerSimple::Sound::Stop()
{
    _IsPlaying = false;
    AudioManagerSimple::Instance()->SfxStop(*this);
}

bool AudioManagerSimple::Sound::IsPlaying() const
{
    return _IsPlaying;
}

const std::string& AudioManagerSimple::Sound::GetName() const
{
    return _Name;
}

bool AudioManagerSimple::Sound::IsLooping() const
{
    return _Looping;
}

void AudioManagerSimple::Sound::SetLooping(bool looping)
{
    _Looping = looping;
    AudioManagerSimple::Instance()->SfxUpdateLooping(*this);
}

float AudioManagerSimple::Sound::GetPitch() const
{
    return _Pitch;
}

void AudioManagerSimple::Sound::SetPitch(float pitch)
{
    _Pitch = pitch;
    AudioManagerSimple::Instance()->SfxUpdatePitch(*this);
}

float AudioManagerSimple::Sound::GetVolume() const
{
    return _Volume;
}

void AudioManagerSimple::Sound::SetVolume(float volume)
{
    _Volume = volume;
    AudioManagerSimple::Instance()->SfxUpdateVolume(*this);
}

AudioManagerSimple::AudioManagerSimple()
{
    
}

AudioManagerSimple::~AudioManagerSimple()
{
    
}

void AudioManagerSimple::Update(float time)
{
    
}

bool AudioManagerSimple::IsBgmMuted()
{
    return false;
}
    
void AudioManagerSimple::MuteBgm(bool mute)
{
    
}

bool AudioManagerSimple::IsSfxMuted()
{
    return false;
}

void AudioManagerSimple::MuteSfx(bool mute)
{
    
}

void AudioManagerSimple::LoadBgm(const std::string& name)
{
    
}

void AudioManagerSimple::LoadSfx(const std::string& name)
{
    
}

void AudioManagerSimple::ReloadSfx()
{
    
}

void AudioManagerSimple::PlayBgm(const std::string& name, bool loop, float volume)
{
    
}
    
void AudioManagerSimple::StopBgm()
{
    
}

AudioManagerSimple::Sound AudioManagerSimple::PlaySfx(const std::string& name, float volume, float pitch)
{
    return Sound(0);
}

void AudioManagerSimple::SfxPlay(Sound& sound)
{
    
}

void AudioManagerSimple::SfxStop(Sound& sound)
{
    
}

bool AudioManagerSimple::SfxIsPlaying(const Sound& sound)
{
    return false;
}

void AudioManagerSimple::SfxUpdateLooping(Sound& sound)
{
    
}

void AudioManagerSimple::SfxUpdatePitch(Sound& sound)
{
    
}

void AudioManagerSimple::SfxUpdateVolume(Sound& sound)
{
    
}
