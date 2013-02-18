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

template<class T> void ResourceHandle<T>::Process()
{
    if (static_cast<T*>(_Resource)->ProcessResource(_State, _Filename, _ErrorDetails))
    {
        switch (_State)
        {
        case kResourceStateLoading:
            _State = kResourceStateProcessing;
            break;
        case kResourceStateProcessing:
            _State = kResourceStatePostProcessing;
            break;
        case kResourceStatePostProcessing:
            _State = kResourceStateComplete;
            break;
        case kResourceStateComplete:
        case kResourceStateError:
            PbAssert(!"Processing a resource in this state should never occur");
            break;
        }
    } else {
        _State = kResourceStateError;
    }
}

template<class T> ResourceThread ResourceHandle<T>::GetThread(ResourceState state)
{
    return T::GetResourceThread(state);
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
