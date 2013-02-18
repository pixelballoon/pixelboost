#pragma once

#include <string>

#include "pixelboost/db/definitions.h"
#include "pixelboost/resource/definitions.h"

namespace pb
{

class Resource
{
public:
    virtual ~Resource();
    
    virtual Uid GetResourceType() = 0;
    virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error) = 0;
    
    // Also required to implement a Resource are the following static methods:
    // * static Uid GetStaticResourceType();
    // * static ResourceThread GetResourceThread(ResourceState state);
};

}