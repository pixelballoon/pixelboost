#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"

using namespace pb;

EffectManager::EffectManager()
{
    
}

EffectManager::~EffectManager()
{
    
}

Effect* EffectManager::LoadEffect(const std::string& file, bool reload)
{
    EffectMap::iterator it = _Effects.find(file);
    
    if (it != _Effects.end())
    {
        if (reload)
        {
            it->second->Load(file);
        }
        
        return it->second;
    }
    
    Effect* effect = new Effect();
    effect->Load(file);
    _Effects[file] = effect;
    return effect;
}

void EffectManager::UnloadEffect(const std::string& file)
{
    EffectMap::iterator it = _Effects.find(file);
    
    if (it != _Effects.end())
    {
        delete it->second;
        _Effects.erase(it);
    }
}

Effect* EffectManager::GetEffect(const std::string& file)
{
    EffectMap::iterator it = _Effects.find(file);
    
    if (it != _Effects.end())
        return it->second;
    
    return 0;
}
