#pragma once

#include <string>

namespace pb
{
    
class ShaderProgram;
    
class Material
{
public:
    Material();
    virtual ~Material();
    
    bool Load(const std::string& filename);
    
    void Bind();
    
    ShaderProgram* GetShaderProgram();
    
private:
    ShaderProgram* _Program;
};

}
