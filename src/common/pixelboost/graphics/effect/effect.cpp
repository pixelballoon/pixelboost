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
    std::string fragmentSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["fragment"]).Value());
    std::string vertexSource = FileHelpers::FileToString(FileHelpers::GetRootPath() + "/" + ((json::String)object["vertex"]).Value());
    
    if (!_Program->Load(fragmentSource, vertexSource))
        return false;
    
    _Program->BindAttribute(kShaderAttributeVertexPosition, "position");
    
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

EffectManager::EffectManager()
{
    
}

EffectManager::~EffectManager()
{
    
}

Effect* EffectManager::LoadEffect(const std::string& filename)
{
    Effect* effect = new Effect();
    effect->Load(filename);
    return effect;
}

#endif
