#include "pixelboost/logic/entity.h"

namespace pb
{

template <class T>T* Scene::GetSystemByType()
{
    SystemMap::iterator it = _Systems.find(T::GetStaticType());
    if (it != _Systems.end())
    {
        return static_cast<T*>(it->second);
    }
    return 0;    
}

template <class T> Scene::EntityMap Scene::GetEntitiesByType()
{
    EntityMap entities;
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetType() == T::GetStaticType())
        {
            entities[it->first] = it->second;
        }
    }
    
    return entities;
}

}
