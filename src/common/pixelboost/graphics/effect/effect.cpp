#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/effect/effect.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

EffectTechnique::EffectTechnique(Uid uid)
    : _Uid(uid)
{
    
}

EffectTechnique::~EffectTechnique()
{
    for (PassList::iterator it = _Passes.begin(); it != _Passes.end(); ++it)
    {
        delete *it;
    }
}

Uid EffectTechnique::GetId()
{
    return _Uid;
}

bool EffectTechnique::Load(json::Array& array)
{
    for (json::Array::iterator it = array.Begin(); it != array.End(); ++it)
    {
        EffectPass* pass = new EffectPass();
        if (!pass->Load(*it))
        {
            delete pass;
            return false;
        }
        
        AddPass(pass);
    }
    
    return true;
}

int EffectTechnique::GetNumPasses()
{
    return _Passes.size();
}

void EffectTechnique::AddPass(EffectPass* pass)
{
    _Passes.push_back(pass);
}

EffectPass* EffectTechnique::GetPass(int index)
{
    if (index < _Passes.size())
        return _Passes[index];
    
    return 0;
}

EffectPass::EffectPass()
{
    _Program = GraphicsDevice::Instance()->CreateProgram();
}

EffectPass::~EffectPass()
{
    GraphicsDevice::Instance()->DestroyProgram(_Program);
}

bool EffectPass::Load(json::Object& object)
{
    json::Object shaders = object["shaders"];
    
    json::Object platform;
    
#ifdef PIXELBOOST_GRAPHICS_OPENGLES2
    platform = shaders["gles2"];
#endif
#ifdef PIXELBOOST_GRAPHICS_OPENGL2
    platform = shaders["gl2"];
#endif
    
    std::string fragmentSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)platform["fragment"]).Value());
    std::string vertexSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)platform["vertex"]).Value());
    
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

void EffectPass::Bind()
{
    GraphicsDevice::Instance()->BindProgram(_Program);
}

ShaderProgram* EffectPass::GetShaderProgram()
{
    return _Program;
}

Effect::Effect()
{
    
}

Effect::~Effect()
{
    for (TechniqueMap::iterator it = _Techniques.begin(); it != _Techniques.end(); ++it)
    {
        delete it->second;
    }
}

bool Effect::Load(const std::string& filename)
{
    std::string effect = FileHelpers::FileToString(FileHelpers::GetRootPath() + filename);
    
    json::Object object;
    if (!json::Reader::Read(object, effect))
        return false;
    
    for (json::Object::iterator it = object.Begin(); it != object.End(); ++it)
    {
        const char* techniqueId = it->name.c_str();
        EffectTechnique* technique = new EffectTechnique(RuntimeTypeHash(techniqueId));
        technique->Load(it->element);
        AddTechnique(technique);
    }
    
    return true;
}

void Effect::AddTechnique(EffectTechnique* technique)
{
    _Techniques[technique->GetId()] = technique;
}

EffectTechnique* Effect::GetTechnique(Uid techniqueId)
{
    TechniqueMap::iterator it = _Techniques.find(techniqueId);
    if (it != _Techniques.end())
        return it->second;
    
    return 0;
}

#endif
