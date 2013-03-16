#include "pixelboost/debug/assert.h"

namespace pb
{

template<class T> ResourceHandle<T>::ResourceHandle(ResourcePool* pool, const std::string& filename, Resource* resource)
    : ResourceHandleBase(pool, filename, resource)
{
}

template<class T> ResourceHandle<T>::~ResourceHandle()
{
}

template<class T> T* ResourceHandle<T>::GetResource()
{
    return static_cast<T*>(_Resource);
}

template<class T> T* ResourceHandle<T>::operator->()
{
    return static_cast<T*>(_Resource);
}

template <class T> std::shared_ptr<ResourceHandle<T> > ResourcePool::GetResource(const std::string& filename)
{
    auto resource = _Resources.find(filename);
    if (resource != _Resources.end())
        return std::static_pointer_cast<ResourceHandle<T> >(resource->second);
    
    auto newResource = std::shared_ptr<ResourceHandle<T> >(new ResourceHandle<T>(this, filename, ResourceManager::Instance()->CreateResourceData(T::GetStaticResourceType())));
    _Resources[filename] = newResource;
    _Pending.push_back(newResource);
    
    return newResource;
}

}
