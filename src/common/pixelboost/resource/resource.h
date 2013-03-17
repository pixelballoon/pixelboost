#pragma once

#include <string>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"

#define PB_DECLARE_RESOURCE public: static pb::Resource* Create(); virtual pb::Uid GetResourceType() const; static pb::Uid GetStaticResourceType();
#define PB_DEFINE_RESOURCE(className) pb::Resource* className::Create() { return new className(); } pb::Uid className::GetResourceType() const { return GetStaticResourceType(); } pb::Uid className::GetStaticResourceType() { return pb::TypeHash(#className); }

namespace pb
{
    
    class ResourcePool;

    class Resource
    {
    protected:
        virtual ~Resource();
        
        virtual Uid GetResourceType() const = 0;
        virtual ResourceReadyState IsReadyToProcess(ResourceProcess process, std::string& errorDetails);
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails) = 0;
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
        friend class ResourceHandleBase;
        friend class ResourceManager;
    };

}
