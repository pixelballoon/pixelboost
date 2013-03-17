#pragma once

#include <string>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"

#define PB_DECLARE_RESOURCE public: static pb::Resource* Create(); virtual pb::Uid GetResourceType() const; static pb::Uid GetStaticResourceType();
#define PB_DEFINE_RESOURCE(className) pb::Resource* className::Create() { return new className(); } pb::Uid className::GetResourceType() const { return GetStaticResourceType(); } pb::Uid className::GetStaticResourceType() { return pb::TypeHash(#className); }

namespace pb
{

class Resource
{
protected:
    virtual ~Resource();
    
    virtual Uid GetResourceType() const = 0;
    virtual ResourceReadyState IsReadyToProcess(ResourceState state, std::string& errorDetails);
    virtual bool ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails) = 0;
    virtual ResourceThread GetResourceThread(ResourceState state) = 0;
    
    friend class ResourceHandleBase;
    friend class ResourceManager;
};

}
