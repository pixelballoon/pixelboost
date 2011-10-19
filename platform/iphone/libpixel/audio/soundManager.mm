#import "ObjectAL.h"

#include "libpixel/audio/soundManager.h"
#include "libpixel/file/fileHelpers.h"

namespace libpixel
{

SoundManager::SoundManager()
{
    _CurrentBgmName = "";
    
    [OALSimpleAudio sharedInstance].allowIpod = NO;
    [OALSimpleAudio sharedInstance].honorSilentSwitch = YES;
}

SoundManager::~SoundManager()
{
    
}
    
void SoundManager::MuteBgm(bool mute)
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

void SoundManager::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void SoundManager::LoadBgm(const std::string& name)
{

}

void SoundManager::LoadSfx(const std::string& name, bool compressed)
{
    //if (compressed)
        return;
    
    std::string fileName = FileHelpers::GetRootPath() + "/data/audio/sfx/" + name + (compressed ? ".mp3" : ".wav");
    
    [[OALSimpleAudio sharedInstance] preloadEffect:[NSString stringWithUTF8String:fileName.c_str()]];
}

void SoundManager::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = FileHelpers::GetRootPath() + "/data/audio/bgm/" + name + ".mp3";
     
    [[OALSimpleAudio sharedInstance] playBg:[NSString stringWithUTF8String:fileName.c_str()] volume:volume pan:0.f loop:loop];
}
    
void SoundManager::StopBgm()
{
    _CurrentBgmName = "";
    
    [[OALSimpleAudio sharedInstance] stopBg];
}

void SoundManager::PlaySfx(const std::string& name, bool compressed, float volume)
{
    if (_MuteSfx)
        return;
    
    std::string fileName = FileHelpers::GetRootPath() + "/data/audio/sfx/" + name + (compressed ? ".mp3" : ".wav");
     
    [[OALSimpleAudio sharedInstance] playEffect:[NSString stringWithUTF8String:fileName.c_str()] volume:volume pitch:1.f pan:0.f loop:false];
}

}