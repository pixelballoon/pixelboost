#include "pixelboost/audio/emscripten/audioManagerSimple.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pb;

AudioManagerSimple* AudioManagerSimple::Instance()
{
    static AudioManagerSimpleEmscripten* instance = new AudioManagerSimpleEmscripten();
    return instance;
}

AudioManagerSimpleEmscripten::AudioManagerSimpleEmscripten()
{
    _CurrentBgmName = "";
    _MuteBgm = false;
    _MuteSfx = false;
}

AudioManagerSimpleEmscripten::~AudioManagerSimpleEmscripten()
{
    
}
    
void AudioManagerSimpleEmscripten::Update(float time)
{
    
}
    
void AudioManagerSimpleEmscripten::MuteBgm(bool mute)
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

void AudioManagerSimpleEmscripten::MuteSfx(bool mute)
{
    _MuteSfx = mute;
}

void AudioManagerSimpleEmscripten::LoadBgm(const std::string& name)
{
}

void AudioManagerSimpleEmscripten::LoadSfx(const std::string& name, bool compressed)
{
}

void AudioManagerSimpleEmscripten::PlayBgm(const std::string& name, bool loop, float volume)
{
    _CurrentBgmName = name;
    _CurrentBgmLoop = loop;
    _CurrentBgmVolume = volume;
    
    if (_MuteBgm || _CurrentBgmName == "")
        return;
    
     std::string fileName = "/data/audio/bgm/" + name + ".mp3";     
}
    
void AudioManagerSimpleEmscripten::StopBgm()
{
    _CurrentBgmName = "";
}

void AudioManagerSimpleEmscripten::PlaySfx(const std::string& name, bool compressed, float volume, float pitch)
{
    if (_MuteSfx)
        return;
    
    std::string fileName = "/data/audio/sfx/" + name + (compressed ? ".mp3" : ".wav");
}
