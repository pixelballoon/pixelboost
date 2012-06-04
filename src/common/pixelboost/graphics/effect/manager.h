#pragma once

#include <map>
#include <string>

namespace pb
{
    
class Effect;

class EffectManager
{
public:
    EffectManager();
    ~EffectManager();
    
    Effect* LoadEffect(const std::string& file, bool reload = false);
    void UnloadEffect(const std::string& file);
    
    Effect* GetEffect(const std::string& file);
    
private:
    typedef std::map<std::string, Effect*> EffectMap;
    EffectMap _Effects;
};

}
