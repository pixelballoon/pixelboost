#include "pixelboost/data/xml/xml.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/shader/shader.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

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
    _Uid = RuntimeTypeHash(technique.attribute("class").value());
    
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

ShaderProgram* ShaderPass::GetShaderProgram()
{
    return _Program;
}

Shader::Shader()
{
    
}

Shader::~Shader()
{
    for (TechniqueMap::iterator it = _Techniques.begin(); it != _Techniques.end(); ++it)
    {
        delete it->second;
    }
}

bool Shader::Load(const std::string& filename)
{
    std::string effect = FileHelpers::FileToString(pb::kFileLocationBundle, filename);
    
    pugi::xml_document document;
    if (!document.load(effect.c_str()))
        return false;

    pugi::xml_node root = document.child("shader");
    
    if (root.empty())
        return false;
    
    pugi::xml_node attributes = root.child("attributes");
    
    pugi::xml_node subShader = root.child("subshader");
    
    while (!subShader.empty())
    {
        ShaderTechnique* technique = new ShaderTechnique();
        technique->Load(attributes, subShader);
        AddTechnique(technique);
        
        subShader = subShader.next_sibling("subshader");
    }
        
    return true;
}

void Shader::AddTechnique(ShaderTechnique* technique)
{
    _Techniques[technique->GetId()] = technique;
}

ShaderTechnique* Shader::GetTechnique(Uid techniqueId)
{
    TechniqueMap::iterator it = _Techniques.find(techniqueId);
    if (it != _Techniques.end())
        return it->second;
    
    return 0;
}

#endif
