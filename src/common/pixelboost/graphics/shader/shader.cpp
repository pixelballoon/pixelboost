#include "pugixml/pugixml.hpp"

#include "pixelboost/debug/log.h"
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
    _Uid = TypeHash(technique.attribute("class").value());
    
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
    
    pugi::xml_node subShader = root.child("subshader");
    
    while (!subShader.empty())
    {
        ShaderTechnique* technique = new ShaderTechnique();
        if (technique->Load(attributes, subShader))
        {
            AddTechnique(technique);
        } else {
            delete technique;
        }
        
        subShader = subShader.next_sibling("subshader");
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
    TechniqueMap::iterator it = _Techniques.find(techniqueId);
    if (it != _Techniques.end())
        return it->second;
    
    return 0;
}

#endif
