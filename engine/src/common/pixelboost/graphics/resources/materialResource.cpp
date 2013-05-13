#include "pixelboost/data/stringHelpers.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/resources/textureResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::MaterialResource)

MaterialResource::MaterialResource(ResourcePool* pool, const std::string& filename)
    : Resource(pool, filename)
{
    _Material = 0;
}

MaterialResource::~MaterialResource()
{
    PbAssert(!_Material);
    PbAssert(!_Shader);
    PbAssert(_Textures.size() == 0);
}

ResourceReadyState MaterialResource::IsReadyToProcess(ResourceProcess process, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        case kResourceProcessProcess:
        case kResourceProcessUnload:
            return kResourceReadyStateReady;
        
        case kResourceProcessPostProcess:
        {
            if (_Shader->GetState() == kResourceStateError)
            {
                errorDetails = "Shader resource failed to load";
                return kResourceReadyStateError;
            }
            
            if (_Shader->GetState() != kResourceStateReady)
            {
                return kResourceReadyStateAwaitingDependencies;
            }
            
            for (auto& texture : _Textures)
            {
                if (texture.second->GetState() == kResourceStateError)
                {
                    errorDetails = "Texture resource failed to load";
                    return kResourceReadyStateError;
                }
                
                if (texture.second->GetState() != kResourceStateReady)
                {
                    return kResourceReadyStateAwaitingDependencies;
                }
            }
                        
            return kResourceReadyStateReady;
        }
    }
}

ResourceError MaterialResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            auto file = FileSystem::Instance()->OpenFile(filename+".mtl");
            if (!file)
            {
                return kResourceErrorNoSuchResource;
            }
            
            file->ReadAll(_FileContents);
            
            return kResourceErrorNone;
        }

        case kResourceProcessProcess:
        {
            _Material = new Material();
            ParseMaterial(pool);
            
            _Shader = pool->GetResource<ShaderResource>(_ShaderFilename);
            if (_Shader->GetState() == kResourceStateReady)
            {
                OnResourceLoaded(_Shader.get(), false);
            }
            _Shader->SignalResourceLoaded.Connect(this, &MaterialResource::OnResourceLoaded);
            _Shader->SignalResourceUnloading.Connect(this, &MaterialResource::OnResourceUnloading);
            
            return kResourceErrorNone;
        }
            
        case kResourceProcessPostProcess:
        {
            return kResourceErrorNone;
        }
        
        case kResourceProcessUnload:
        {
            if (_Material)
            {
                delete _Material;
                _Material = 0;
            }
            if (_Shader)
            {
                _Shader->SignalResourceLoaded.Disconnect(this, &MaterialResource::OnResourceLoaded);
                _Shader->SignalResourceUnloading.Disconnect(this, &MaterialResource::OnResourceUnloading);
                _Shader.reset();
            }
            for (auto& texture : _Textures)
            {
                texture.second->SignalResourceLoaded.Disconnect(this, &MaterialResource::OnResourceLoaded);
                texture.second->SignalResourceUnloading.Disconnect(this, &MaterialResource::OnResourceUnloading);
            }
            _Textures.clear();
            return kResourceErrorNone;
        }
    }
}

ResourceThread MaterialResource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadMain;
}

Material* MaterialResource::GetMaterial()
{
    return _Material;
}

void MaterialResource::ParseMaterial(ResourcePool* pool)
{
    std::vector<std::string> lines;
    StringHelpers::SplitString(_FileContents, '\n', lines);
    for (const auto& line : lines)
    {
        if (line[0] == '#')
            continue;
        
        std::string param = line.substr(0, line.find_first_of('('));
        
        std::vector<std::string> values;
        StringHelpers::SplitString(line.substr(line.find_first_of('(')+1, line.find_last_of(')')-param.length()-1), ';', values);
        
        for (auto& value : values)
        {
            StringHelpers::Trim(value);
        }
        
        if (param == "shader")
        {
            _ShaderFilename = values[0];
        }
        
        if (param == "blend")
        {
            _Material->SetBlendEnabled(ParseBoolValue(values[0]));
        }
        
        if (param == "blend_mode")
        {
            PbAssert(values.size() == 2);
            _Material->SetBlendMode(ParseBlendValue(values[0]), ParseBlendValue(values[1]));
        }
        
        if (param == "depth")
        {
            _Material->SetDepthTestEnabled(ParseBoolValue(values[0]));
        }
        
        if (param == "property")
        {
            std::string propertyName = values[1];
            std::string propertyValue = values[2];
            if (values[0] == "texture2D")
            {
                _Material->AddProperty(propertyName, ShaderProperty::kPropertyTexture2D);
                PbAssert(_Textures.find(propertyName) == _Textures.end());
                _Textures[propertyName] = pool->GetResource<pb::TextureResource>(propertyValue);
                _Textures[propertyName]->SignalResourceLoaded.Connect(this, &MaterialResource::OnResourceLoaded);
                _Textures[propertyName]->SignalResourceUnloading.Connect(this, &MaterialResource::OnResourceUnloading);
                if (_Textures[propertyName]->GetState() == kResourceStateReady)
                {
                    OnResourceLoaded(_Textures[propertyName].get(), false);
                }
            }
        }
    }
}

bool MaterialResource::ParseBoolValue(const std::string& value)
{
    if (value == "on")
    {
        return true;
    } else if (value == "off")
    {
        return false;
    }
    
    PbAssert(!"Unable to parse bool value, expected on/off");
    
    return false;
}

GraphicsDevice::Blend MaterialResource::ParseBlendValue(const std::string& value)
{
    if (value == "one")
    {
        return GraphicsDevice::kBlendOne;
    } else if (value == "source_alpha")
    {
        return GraphicsDevice::kBlendSourceAlpha;
    } else if (value == "one_minus_source_alpha")
    {
        return GraphicsDevice::kBlendOneMinusSourceAlpha;
    }
    
    PbAssert(!"Unable to parse blend value, expected one/source_alpha/one_minus_source_alpha");
    
    return GraphicsDevice::kBlendOne;
}

void MaterialResource::OnResourceLoaded(Resource* resource, bool error)
{
    if (error)
        return;
    
    if (resource == _Shader.get())
    {
        _Material->SetShader(_Shader->GetShader());
    }
    
    for (auto& texture : _Textures)
    {
        if (resource == texture.second.get())
        {
            _Material->GetProperty(texture.first)->SetValue(texture.second->GetTexture());
        }
    }
}

void MaterialResource::OnResourceUnloading(Resource* resource)
{
    if (resource == _Shader.get())
    {
        _Material->SetShader(0);
    }
    
    for (auto& texture : _Textures)
    {
        if (resource == texture.second.get())
        {
            _Material->GetProperty(texture.first)->SetValue((Texture*)0);
        }
    }
}
