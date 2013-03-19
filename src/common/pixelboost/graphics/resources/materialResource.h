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
        virtual ResourceReadyState IsReadyToProcess(ResourceProcess process, std::string& errorDetails);
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
        Material* GetMaterial();
        
    private:
        void OnResourceLoaded(ResourceHandleBase* resource, bool error);
        void OnResourceUnloading(ResourceHandleBase* resource);
        
        Material* _Material;
        std::shared_ptr<ResourceHandle<ShaderResource> > _Shader;
    };
    
}
