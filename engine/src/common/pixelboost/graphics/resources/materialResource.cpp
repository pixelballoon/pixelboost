#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/graphics/resources/shaderResource.h"

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
}

ResourceReadyState MaterialResource::IsReadyToProcess(ResourceProcess process, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        case kResourceProcessPostProcess:
        case kResourceProcessUnload:
            return kResourceReadyStateReady;
            
        case kResourceProcessProcess:
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
            _Material = new Material();
            _Shader = pool->GetResource<ShaderResource>(filename.substr(0, filename.length()-1));
            if (_Shader->GetState() == kResourceStateReady)
            {
                OnResourceLoaded(_Shader.get(), false);
            }
            _Shader->SignalResourceLoaded.Connect(this, &MaterialResource::OnResourceLoaded);
            _Shader->SignalResourceUnloading.Connect(this, &MaterialResource::OnResourceUnloading);
            return kResourceErrorNone;
        }

        case kResourceProcessProcess:
        {
            _Material->SetShader(_Shader->GetShader());
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

void MaterialResource::OnResourceLoaded(Resource* resource, bool error)
{
    if (resource == _Shader.get())
    {
        _Material->SetShader(_Shader->GetShader());
    }
}

void MaterialResource::OnResourceUnloading(Resource* resource)
{
    if (resource == _Shader.get())
    {
        _Material->SetShader(0);
    }
}