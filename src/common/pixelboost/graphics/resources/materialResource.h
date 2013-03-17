#pragma once

#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class Material;
    
    class MaterialResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        MaterialResource();
        ~MaterialResource();
        
    public:
        virtual bool ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
        Material* GetMaterial();
        
    private:
        Material* _Material;
    };
    
}
