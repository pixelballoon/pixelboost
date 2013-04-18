#include "pixelboost/resource/resourceSet.h"

using namespace pb;

ResourceSet::ResourceSet()
{
    
}

ResourceSet::~ResourceSet()
{
    
}

bool ResourceSet::IsLoaded()
{
    for (const auto& resource : _Resources)
    {
        if (resource.second->GetState() != kResourceStateReady)
        {
            return false;
        }
    }
    
    return true;
}

float ResourceSet::GetProgress()
{
    if (_Resources.size() == 0)
        return 1.f;
    
    int resourcesReady = 0;
    for (const auto& resource : _Resources)
    {
        if (resource.second->GetState() == kResourceStateReady)
        {
            resourcesReady++;
        }
    }
    
    return (float)resourcesReady/(float)_Resources.size();
}
