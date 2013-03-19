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

ShaderTechnique::ShaderTechnique()
    : _Uid(-1)
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

bool ShaderTechnique::Load(const pugi::xml_node& attributes, const pugi::xml_node& technique)
{
    _Uid = TypeHash(technique.attribute("name").value());
    
    pugi::xml_node pass = technique.child("pass");
    while (!pass.empty())
    {
        ShaderPass* shaderPass = new ShaderPass();
        if (!shaderPass->Load(attributes, pass))
        {
            delete shaderPass;
            return false;
        }
        
        AddPass(shaderPass);
        
        pass = pass.next_sibling("pass");
    }
    
    return true;
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

bool ShaderPass::Load(const pugi::xml_node& attributes, const pugi::xml_node& pass)
{
    if (!_Program->Load(attributes, pass))
        return false;
    
    if (!_Program->Link())
        return false;

    return true;
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

bool Shader::Load(const std::string& filename)
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
    
    std::string effect;
    
    if (!file)
    {
        PbLogError("pb.graphics.shader", "Failed to load shader %s, unable to open file", filename.c_str());
        return false;
    }
    
    file->ReadAll(effect);
    delete file;
    
    pugi::xml_document document;
    if (!document.load(effect.c_str()))
    {
        PbLogError("pb.graphics.shader", "Failed to parse shader %s, xml parse error", filename.c_str());
        return false;
    }

    pugi::xml_node root = document.child("shader");
    
    if (root.empty())
    {
        PbLogError("pb.graphics.shader", "Failed to load shader %s, root node is empty", filename.c_str());
        return false;
    }
    
    pugi::xml_node attributes = root.child("attributes");
    
    pugi::xml_node techniques = root.child("technique");
    
    while (!techniques.empty())
    {
        ShaderTechnique* technique = new ShaderTechnique();
        if (technique->Load(attributes, techniques))
        {
            AddTechnique(technique);
        } else {
            delete technique;
        }
        
        techniques = techniques.next_sibling("technique");
    }
    
    if (_Techniques.size() == 0)
    {
        PbLogError("pb.graphics.shader", "Failed to load shader %s, no techniques were loaded (are they compatible with this card?)", filename.c_str());
        return false;
    }
        
    return true;
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

const std::map<std::string, ShaderProperty::PropertyType>& Shader::GetProperties()
{
    return _Properties;
}
