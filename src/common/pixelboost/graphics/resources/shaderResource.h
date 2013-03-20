#pragma once

#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class Shader;
    
    class ShaderResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        ShaderResource(ResourcePool* pool, const std::string& filename);
        ~ShaderResource();
        
    public:
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
        Shader* GetShader();
        
    private:
        Shader* _Shader;
    };
    
}
