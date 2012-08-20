#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/db/definitions.h"

namespace json
{
    class Array;
    class Object;
}

namespace pb
{

class ShaderPass;
class ShaderProgram;
    
class ShaderTechnique
{
public:
    ShaderTechnique(Uid uid);
    ~ShaderTechnique();
    
    Uid GetId();
    
    bool Load(json::Array& object);
    
    int GetNumPasses();
    void AddPass(ShaderPass* pass);
    ShaderPass* GetPass(int index);
    
private:
    typedef std::vector<ShaderPass*> PassList;
    
private:
    Uid _Uid;
    PassList _Passes;
};
    
class ShaderPass
{
public:
    ShaderPass();
    ~ShaderPass();
    
    bool Load(json::Object& object);
    
    void Bind();
    
    ShaderProgram* GetShaderProgram();
    
private:
    ShaderProgram* _Program;
};
    
class Shader
{
public:
    Shader();
    ~Shader();
    
    bool Load(const std::string& filename);
    
    void AddTechnique(ShaderTechnique* technique);
    ShaderTechnique* GetTechnique(Uid techniqueId);
    
private:
    typedef std::map<Uid, ShaderTechnique*> TechniqueMap;
    
    TechniqueMap _Techniques;
};

}
