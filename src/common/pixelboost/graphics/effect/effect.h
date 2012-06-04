#pragma once

#include <string>

#include "pixelboost/db/definitions.h"

namespace json
{
    class Array;
    class Object;
}

namespace pb
{

class EffectPass;
class ShaderProgram;
    
class EffectTechnique
{
public:
    EffectTechnique(Uid uid);
    ~EffectTechnique();
    
    Uid GetId();
    
    bool Load(json::Array& object);
    
    void AddPass(EffectPass* pass);
    EffectPass* GetPass(int index);
    
private:
    typedef std::vector<EffectPass*> PassList;
    
private:
    Uid _Uid;
    PassList _Passes;
};
    
class EffectPass
{
public:
    EffectPass();
    ~EffectPass();
    
    bool Load(json::Object& object);
    
    void Bind();
    
    ShaderProgram* GetShaderProgram();
    
private:
    ShaderProgram* _Program;
};
    
class Effect
{
public:
    Effect();
    ~Effect();
    
    bool Load(const std::string& filename);
    
    void AddTechnique(EffectTechnique* technique);
    EffectTechnique* GetTechnique(Uid techniqueId);
    
private:
    typedef std::map<Uid, EffectTechnique*> TechniqueMap;
    
    TechniqueMap _Techniques;
};
    
class EffectManager
{
public:
    EffectManager();
    ~EffectManager();
    
    Effect* LoadEffect(const std::string& filename);
};

}
