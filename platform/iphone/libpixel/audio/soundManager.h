#ifndef LIBPIXEL__AUDIO__SOUNDMANAGER__H
#define LIBPIXEL__AUDIO__SOUNDMANAGER__H

#include <string>

namespace libpixel
{

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();
    
    void MuteBgm(bool mute=true);
    void MuteSfx(bool mute=true);
    
    void LoadBgm(const std::string& name);
    void LoadSfx(const std::string& name, bool compressed);
    
    void PlayBgm(const std::string& name, bool loop, float volume=1.f);
    void StopBgm();
    
    void PlaySfx(const std::string& name, bool compressed, float volume=1.f);
    
private:
    std::string _CurrentBgmName;
    bool _CurrentBgmLoop;
    bool _CurrentBgmVolume;
    
    bool _MuteBgm;
    bool _MuteSfx;
};
    
}

#endif