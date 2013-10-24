#ifdef PIXELBOOST_PLATFORM_IOS

#ifndef PIXELBOOST_DISABLE_SOUND

#include <vector>

#import "ObjectAL.h"
#import "ALSoundSource.h"

#include "pixelboost/audio/ios/audioManagerSimple.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pb;

AudioManagerSimple* AudioManagerSimple::Instance()
{
    static AudioManagerSimpleIOS* instance = new AudioManagerSimpleIOS();
    return instance;
}

AudioManagerSimpleIOS::AudioManagerSimpleIOS()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
    
    [OALSimpleAudio sharedInstance].allowIpod = YES;
    [OALSimpleAudio sharedInstance].honorSilentSwitch = YES;
}

AudioManagerSimpleIOS::~AudioManagerSimpleIOS()
{
    
}

void AudioManagerSimpleIOS::Update(float time)
{
    std::vector<int> inactiveSounds;
    
    for (std::map<int, void*>::iterator it = _Sounds.begin(); it != _Sounds.end(); ++it)
    {
        id<ALSoundSource> src = (id<ALSoundSource>)it->second;
        
        if (src.playing == false)
            inactiveSounds.push_back(it->first);
    }
    
    for (std::vector<int>::iterator it = inactiveSounds.begin(); it != inactiveSounds.end(); ++it)
    {
        _Sounds.erase(*it);
    }
}

bool AudioManagerSimpleIOS::IsBgmMuted()
{
    return _MuteBgm;
}
    
void AudioManagerSimpleIOS::MuteBgm(bool mute)
{
    _MuteBgm = mute;
    
    if (_MuteBgm)
    {
        std::string currentBgm = _CurrentBgmName;
        StopBgm();
        _CurrentBgmName = currentBgm;
    } else {
        PlayBgm(_CurrentBgmName, _CurrentBgmLoop, _CurrentBgmVolume);
    }
}

bool AudioManagerSimpleIOS::IsSfxMuted()
{
    return _MuteSfx;
}

void AudioManagerSimpleIOS::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void AudioManagerSimpleIOS::LoadBgm(const std::string& name)
{

}

void AudioManagerSimpleIOS::LoadSfx(const std::string& name)
{
    std::string fileName = FileHelpers::GetBundlePath() + "/data/audio/sfx/" + name;
    
    [[OALSimpleAudio sharedInstance] preloadEffect:[NSString stringWithUTF8String:fileName.c_str()]];
}

void AudioManagerSimpleIOS::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = FileHelpers::GetBundlePath() + "/data/audio/bgm/" + name;
     
    [[OALSimpleAudio sharedInstance] playBg:[NSString stringWithUTF8String:fileName.c_str()] volume:volume pan:0.f loop:loop];
}
    
void AudioManagerSimpleIOS::StopBgm()
{
    _CurrentBgmName = "";
    
    [[OALSimpleAudio sharedInstance] stopBg];
}

AudioManagerSimple::Sound AudioManagerSimpleIOS::PlaySfx(const std::string& name, float volume, float pitch)
{
    if (_MuteSfx)
        return Sound(0);
    
    Sound sound(SfxGetId(), name, volume, pitch);
    
    sound.Play();
    
    return sound;
}

int AudioManagerSimpleIOS::SfxGetId()
{
    return _SoundId++;
}

void AudioManagerSimpleIOS::SfxPlay(Sound& sound)
{
    if (_MuteSfx)
        return;
    
    std::string filename = FileHelpers::GetBundlePath() + "/data/audio/sfx/" + sound.GetName();
    
    id<ALSoundSource> instance = [[OALSimpleAudio sharedInstance] playEffect:[NSString stringWithUTF8String:filename.c_str()] volume:sound.GetVolume() pitch:sound.GetPitch() pan:0.f loop:sound.IsLooping()];
    [instance retain];
    _Sounds[sound.GetId()] = instance;
}

void AudioManagerSimpleIOS::SfxStop(Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    [src stop];
}

bool AudioManagerSimpleIOS::SfxIsPlaying(const Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return false;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    return src.playing;
}

void AudioManagerSimpleIOS::SfxUpdateLooping(Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.looping = sound.IsLooping();
}

void AudioManagerSimpleIOS::SfxUpdatePitch(Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.pitch = sound.GetPitch();
}

void AudioManagerSimpleIOS::SfxUpdateVolume(Sound& sound)
{
    std::map<int, void*>::iterator it = _Sounds.find(sound.GetId());
    
    if (it == _Sounds.end())
        return;
    
    id<ALSoundSource> src = (id<ALSoundSource>)it->second;
    
    src.volume = sound.GetVolume();
}

#endif

#endif
