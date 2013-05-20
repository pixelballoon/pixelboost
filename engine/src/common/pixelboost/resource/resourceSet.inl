#include "pixelboost/resource/resourceManager.h"

namespace pb
{

template <class T> std::shared_ptr<T> ResourceSet::AddResource(const std::string& resource)
{
    _Resources[resource] = ResourceManager::Instance()->GetPool("default")->GetResource<T>(resource);
    
    return std::static_pointer_cast<T>(_Resources[resource]);
}

}
