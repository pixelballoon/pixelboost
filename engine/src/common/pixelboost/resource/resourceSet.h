#pragma once

#include <map>
#include <memory>
#include <string>

namespace pb
{
    
    class Resource;
    
    class ResourceSet
    {
    public:
        ResourceSet();
        ~ResourceSet();
        
        template<typename T> std::shared_ptr<T> AddResource(const std::string& resource, const std::string& pool = "default");
        
        bool IsLoaded();
        float GetProgress();
        
    private:
        std::map<std::string, std::shared_ptr<Resource>> _Resources;
    };
    
}

#include "pixelboost/resource/resourceSet.inl"
