#pragma once

#include "pugixml/pugixml.hpp"

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
        bool ParseShader(std::string& errorDetails);
        
        pugi::xml_document _Document;
        Shader* _Shader;
    };
    
}
