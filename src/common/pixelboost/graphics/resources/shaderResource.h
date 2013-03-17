#pragma once

#include "pixelboost/resource/resource.h"

namespace pb
{
    
    class Shader;
    
    class ShaderResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        ShaderResource();
        ~ShaderResource();
        
    public:
        virtual bool ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
        Shader* GetShader();
        
    private:
        Shader* _Shader;
    };
    
}
