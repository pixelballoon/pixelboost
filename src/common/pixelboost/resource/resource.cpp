#include "pixelboost/resource/resource.h"

using namespace pb;

Resource::~Resource()
{
    
}

ResourceReadyState Resource::IsReadyToProcess(ResourceState state, std::string& errorDetails)
{
    return kResourceReadyStateReady;
}