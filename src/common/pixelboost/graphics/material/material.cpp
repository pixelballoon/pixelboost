#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

Material::Material()
{
    
}

Material::~Material()
{
    
}

Shader* Material::GetShader()
{
    return _Shader;
}

void Material::SetShader(Shader* shader)
{
    _Shader = shader;
}

const std::map<std::string, ShaderProperty>& Material::GetProperties()
{
    return _Properties;
}

ShaderProperty* Material::GetProperty(const std::string& name)
{
    auto it = _Properties.find(name);
    
    if (it != _Properties.end())
    {
        return &it->second;
    }
    
    return 0;
}

void Material::Bind(Uid techniqueId, int pass, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, float gameTime, float realTime)
{
    
}

int Material::GetNumPasses(Uid techniqueId)
{
    if (_Shader)
    {
        ShaderTechnique* technique = _Shader->GetTechnique(techniqueId);
        
        if (technique)
        {
            return technique->GetNumPasses();
        }
    }
    
    return 0;
}
