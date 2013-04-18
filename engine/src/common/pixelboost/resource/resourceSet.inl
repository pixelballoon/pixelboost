#include "pixelboost/resource/resourceManager.h"

namespace pb
{

template <class T> std::shared_ptr<T> ResourceSet::AddResource(const std::string& resource, const std::string& pool)
{
    _Resources[resource] = ResourceManager::Instance()->GetPool(pool)->GetResource<T>(resource);
    
    return std::static_pointer_cast<T>(_Resources[resource]);
}

}
