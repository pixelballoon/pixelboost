#include "pugixml/pugixml.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

ShaderProperty::ShaderProperty()
{
    _Type = kPropertyUnknown;
}

ShaderProperty::ShaderProperty(PropertyType type)
{
    _Type = type;
    
    switch (_Type)
    {
        case kPropertyUnknown:
            break;
        case kPropertyFloat:
            _Value.Float = 0.f;
            break;
        case kPropertyMat44:
            memset(&_Value.Mat44, 0, sizeof(_Value.Mat44));
            break;
        case kPropertyVec4:
            memset(&_Value.Vec4, 0, sizeof(_Value.Vec4));
            break;
        case kPropertyTexture2D:
            _Value.Texture = 0;
            break;
    }
}

ShaderProperty::~ShaderProperty()
{
    
}

ShaderProperty::PropertyType ShaderProperty::GetType() const
{
    return _Type;
}

float ShaderProperty::GetFloatValue() const
{
    return _Value.Float;
}

Texture* ShaderProperty::GetTextureValue() const
{
    return _Value.Texture;
}

void ShaderProperty::SetValue(float value)
{
    _Value.Float = value;
}

void ShaderProperty::SetValue(Texture* texture)
{
    _Value.Texture = texture;
}

ShaderTechnique::ShaderTechnique(Uid techniqueId)
    : _Uid(techniqueId)
{
    
}

ShaderTechnique::~ShaderTechnique()
{
    for (PassList::iterator it = _Passes.begin(); it != _Passes.end(); ++it)
    {
        delete *it;
    }
}

Uid ShaderTechnique::GetId()
{
    return _Uid;
}

int ShaderTechnique::GetNumPasses()
{
    return _Passes.size();
}

void ShaderTechnique::AddPass(ShaderPass* pass)
{
    _Passes.push_back(pass);
}

ShaderPass* ShaderTechnique::GetPass(int index)
{
    if (index < _Passes.size())
        return _Passes[index];
    
    return 0;
}

ShaderPass::ShaderPass()
{
    _Program = GraphicsDevice::Instance()->CreateProgram();
}

ShaderPass::~ShaderPass()
{
    GraphicsDevice::Instance()->DestroyProgram(_Program);
}

void ShaderPass::Bind()
{
    GraphicsDevice::Instance()->BindProgram(_Program);
}

void ShaderPass::SetEngineUniforms(const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, float realTime, float gameTime)
{
    _Program->SetUniform("PB_GameTime", gameTime);
    _Program->SetUniform("PB_RealTime", realTime);
    _Program->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    _Program->SetUniform("PB_ViewMatrix", viewMatrix);
}

ShaderProgram* ShaderPass::GetShaderProgram()
{
    return _Program;
}

Shader::Shader()
{
    
}

Shader::~Shader()
{
    for (const auto& technique : _Techniques)
    {
        delete technique.second;
    }
}

void Shader::AddTechnique(ShaderTechnique* technique)
{
    _Techniques[technique->GetId()] = technique;
}

ShaderTechnique* Shader::GetTechnique(Uid techniqueId)
{
    auto it = _Techniques.find(techniqueId);
    if (it != _Techniques.end())
        return it->second;
    
    return Renderer::Instance()->GetTechnique(techniqueId);
}

int Shader::GetNumTechniques()
{
    return _Techniques.size();
}

const std::map<std::string, ShaderProperty::PropertyType>& Shader::GetProperties()
{
    return _Properties;
}
