#include "pixelboost/graphics/material/material.h"
#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/graphics/resources/shaderResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::MaterialResource)

MaterialResource::MaterialResource()
{
    _Material = 0;
}

MaterialResource::~MaterialResource()
{
    
}

ResourceReadyState MaterialResource::IsReadyToProcess(ResourceState state, std::string& errorDetails)
{
    switch (state)
    {
        case kResourceStateLoading:
            return kResourceReadyStateReady;
        case kResourceStateProcessing:
            {
                if (_Shader->GetState() == kResourceStateError)
                {
                    errorDetails = "Shader resource failed to load";
                    return kResourceReadyStateError;
                }
                
                if (_Shader->GetState() != kResourceStateComplete)
                {
                    return kResourceReadyStateAwaitingDependencies;
                }
                
                return kResourceReadyStateReady;
            }
        case kResourceStateError:
        case kResourceStateComplete:
        case kResourceStatePostProcessing:
        case kResourceStateUnloading:
            return kResourceReadyStateReady;
    }
}

bool MaterialResource::ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails)
{
    switch (state)
    {
        case kResourceStateLoading:
        {
            _Material = new Material();
            _Shader = ResourceManager::Instance()->GetPool("pb::default")->GetResource<ShaderResource>(filename.substr(0, filename.length()-1));
            return true;
        }

        case kResourceStateProcessing:
        {
            _Material->SetShader(_Shader->GetResource()->GetShader());
            return true;
        }
        
        case kResourceStateUnloading:
        {
            if (_Material)
            {
                delete _Material;
                _Material = 0;
            }
            _Shader.reset();
            return true;
        }
            
        case kResourceStateComplete:
        case kResourceStateError:
        case kResourceStatePostProcessing:
            break;
    }

    return true;
}

ResourceThread MaterialResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadMain;
}

Material* MaterialResource::GetMaterial()
{
    return _Material;
}
