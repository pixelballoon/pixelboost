#pragma once

#include "pixelboost/resource/resourceManager.h"

namespace pb
{
    
    class Material;
    class ShaderResource;
    
    class MaterialResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        MaterialResource();
        ~MaterialResource();
        
    public:
        virtual ResourceReadyState IsReadyToProcess(ResourceState state, std::string& errorDetails);
        virtual bool ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
        Material* GetMaterial();
        
    private:
        Material* _Material;
        std::shared_ptr<ResourceHandle<ShaderResource> > _Shader;
        
    };
    
}
