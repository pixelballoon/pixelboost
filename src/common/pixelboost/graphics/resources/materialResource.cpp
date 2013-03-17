#include "pixelboost/graphics/resources/materialResource.h"

using namespace pb;

MaterialResource::MaterialResource()
{
    
}

MaterialResource::~MaterialResource()
{
    
}

bool MaterialResource::ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails)
{
    error = kResourceErrorSystemError;
    errorDetails = "Loader not yet implemented";
    return true;
}

ResourceThread MaterialResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadAny;
}

Material* MaterialResource::GetMaterial()
{
    return _Material;
}
