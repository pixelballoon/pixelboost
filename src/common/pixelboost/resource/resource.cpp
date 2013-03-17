#include "pixelboost/resource/resource.h"

using namespace pb;

Resource::~Resource()
{
    
}

ResourceReadyState Resource::IsReadyToProcess(ResourceProcess process, std::string& errorDetails)
{
    return kResourceReadyStateReady;
}

ResourceThread Resource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadAny;
}
