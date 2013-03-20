#include "pixelboost/debug/assert.h"

namespace pb
{

template <class T> std::shared_ptr<T> ResourcePool::GetResource(const std::string& filename)
{
    auto resource = _Resources.find(filename);
    if (resource != _Resources.end())
    {
        if (auto resourceHandle = resource->second.lock())
        {
            return std::static_pointer_cast<T>(resourceHandle);
        }
    }
    
    auto newResource = std::shared_ptr<T>(new T(this, filename), &ResourcePool::ResourceDeallocator);
    _Resources[filename] = newResource;
    _Pending.push_back(newResource);
    
    return newResource;
}

template <class T> void ResourcePool::CacheResource(const std::string& filename)
{
    auto resource = _Resources.find(filename);
    
    if (resource != _Resources.end())
    {
        if (auto resourceHandle = resource->second.lock())
        {
            _CachedResources[filename] = resourceHandle;
            return;
        }
    }
    
    _CachedResources[filename] = GetResource<T>(filename);
}

}
