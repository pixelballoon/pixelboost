#include "pixelboost/debug/assert.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

Material::Material()
{
    _Blend = true;
    _BlendMode.first = GraphicsDevice::kBlendOne;
    _BlendMode.second = GraphicsDevice::kBlendOneMinusSourceAlpha;
    _DepthTest = true;
    _Shader = 0;
}

Material::~Material()
{
    
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
    pass->SetEngineUniforms(projectionMatrix, viewMatrix, Engine::Instance()->GetTotalTime(), Engine::Instance()->GetGameTime());
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, _Blend);
    GraphicsDevice::Instance()->SetBlendMode(_BlendMode.first, _BlendMode.second);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, _DepthTest);
    
    ShaderProgram* program = pass->GetShaderProgram();
    int textureUnit = 0;
    for (auto& property : _Properties)
    {
        switch (property.second.GetType())
        {
            case ShaderProperty::kPropertyTexture2D:
            {
                program->SetUniform(property.first, textureUnit);
                GraphicsDevice::Instance()->BindTexture(textureUnit, property.second.GetTextureValue());
                textureUnit++;
                break;
            }
            case ShaderProperty::kPropertyFloat:
            case ShaderProperty::kPropertyMat44:
            case ShaderProperty::kPropertyVec4:
                PbAssert(!"Property value of this type isn't yet implemented");
                break;
            case ShaderProperty::kPropertyUnknown:
                PbAssert(!"Unknown property value");
                break;
        }
    }
    
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

Shader* Material::GetShader()
{
    return _Shader;
}

void Material::SetShader(Shader* shader)
{
    _Shader = shader;
}

bool Material::GetBlendEnabled()
{
    return _Blend;
}

void Material::SetBlendEnabled(bool enabled)
{
    _Blend = enabled;
}

std::pair<GraphicsDevice::Blend, GraphicsDevice::Blend> Material::GetBlendMode()
{
    return _BlendMode;
}

void Material::SetBlendMode(GraphicsDevice::Blend source, GraphicsDevice::Blend destination)
{
    _BlendMode.first = source;
    _BlendMode.second = destination;
}

bool Material::GetDepthTestEnabled()
{
    return _DepthTest;
}

void Material::SetDepthTestEnabled(bool enabled)
{
    _DepthTest = enabled;
}

ShaderProperty& Material::AddProperty(const std::string& name, ShaderProperty::PropertyType type)
{
    _Properties[name] = ShaderProperty(type);
    return _Properties[name];
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

