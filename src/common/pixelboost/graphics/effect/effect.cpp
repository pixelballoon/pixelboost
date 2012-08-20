#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/effect/effect.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

ShaderTechnique::ShaderTechnique(Uid uid)
    : _Uid(uid)
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

bool ShaderTechnique::Load(json::Array& array)
{
    for (json::Array::iterator it = array.Begin(); it != array.End(); ++it)
    {
        ShaderPass* pass = new ShaderPass();
        if (!pass->Load(*it))
        {
            delete pass;
            return false;
        }
        
        AddPass(pass);
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

bool ShaderPass::Load(json::Object& object)
{
    json::Object shaders = object["shaders"];
    
    json::Object platform;
    
#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
    platform = shaders["gles2"];
#endif
#ifdef PIXELBOOST_GRAPHICS_OPENGL2
    platform = shaders["gl2"];
#endif
    
    std::string fragmentSource = FileHelpers::FileToString(pb::kFileLocationBundle, "/" + ((json::String)platform["fragment"]).Value());
    std::string vertexSource = FileHelpers::FileToString(pb::kFileLocationBundle, "/" + ((json::String)platform["vertex"]).Value());
    
    if (!_Program->Load(fragmentSource, vertexSource))
        return false;
    
    json::Object& attributes = object["attributes"];
    for (json::Object::iterator it = attributes.Begin(); it != attributes.End(); ++it)
    {
        _Program->BindAttribute((json::Number)it->element, it->name);
    }
    
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
    
    json::Object object;
    if (!json::Reader::Read(object, effect))
        return false;
    
    for (json::Object::iterator it = object.Begin(); it != object.End(); ++it)
    {
        const char* techniqueId = it->name.c_str();
        ShaderTechnique* technique = new ShaderTechnique(RuntimeTypeHash(techniqueId));
        technique->Load(it->element);
        AddTechnique(technique);
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
