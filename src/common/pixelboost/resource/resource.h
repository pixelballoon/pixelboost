#pragma once

#include <string>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"

#define PB_DECLARE_RESOURCE public: virtual pb::Uid GetResourceType() const; static pb::Uid GetStaticResourceType();
#define PB_DEFINE_RESOURCE(className) pb::Uid className::GetResourceType() const { return GetStaticResourceType(); } pb::Uid className::GetStaticResourceType() { return pb::TypeHash(#className); }

namespace pb
{

class Resource
{
public:
    virtual ~Resource();
    
    virtual Uid GetResourceType() const = 0;
    virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error) = 0;
    virtual ResourceThread GetResourceThread(ResourceState state) = 0;
};

}
