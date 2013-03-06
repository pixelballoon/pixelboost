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

template <class T>T* Scene::CreateEntity(Entity* parent, DbEntity* creationEntity)
{
    T* entity = static_cast<T*>(T::Create(this, parent, creationEntity));
    AddEntity(entity);
    return entity;
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
    
    for (EntityMap::iterator it = _NewEntities.begin(); it != _NewEntities.end(); ++it)
    {
        if (it->second->GetType() == T::GetStaticType())
        {
            entities[it->first] = it->second;
        }
    }
    
    return entities;
}

}
