#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

Material::Material()
{
    _Shader = 0;
    for (int i=0; i<8; i++)
    {
        _Textures[i] = 0;
    }
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
    _Properties.clear();
    _Shader = shader;
    
    for (const auto& property : _Shader->GetProperties())
    {
        _Properties[property.first] = ShaderProperty(property.second);
    }
}

Texture* Material::GetTexture(int textureIndex)
{
    return _Textures[textureIndex];
}

void Material::SetTexture(int textureIndex, Texture* texture)
{
    _Textures[textureIndex] = texture;
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

ShaderPass* Material::Bind(Uid techniqueId, int passIndex, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    if (!_Shader)
        return 0;
    
    ShaderTechnique* technique = _Shader->GetTechnique(techniqueId);
    
    if (!technique)
        return 0;

    ShaderPass* pass = technique->GetPass(passIndex);
    pass->Bind();
    
    ShaderProgram* program = pass->GetShaderProgram();
    program->SetUniform("PB_GameTime", Engine::Instance()->GetGameTime());
    program->SetUniform("PB_RealTime", Engine::Instance()->GetTotalTime());
    program->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    program->SetUniform("PB_ViewMatrix", viewMatrix);
    
    return pass;
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
