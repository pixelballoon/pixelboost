#include "pixelboost/db/entity.h"
#include "pixelboost/logic/component.h"

namespace pb
{

template<class T> const T* Entity::GetData() const
{
    if (!_CreationEntity)
        return 0;
        
    return _CreationEntity->GetData<T>();
}

template <class T>T* Entity::GetComponentByType()
{
    Entity::ComponentList components = GetComponentsByType<T>();
    
    if (components.size())
    {
        return static_cast<T*>(components.at(0));
    }
    
    return 0;
}

template <class T> Entity::ComponentList Entity::GetComponentsByType()
{
    ComponentList components;
    
    for (ComponentList::iterator componentIt = _Components.begin(); componentIt != _Components.end(); ++componentIt)
    {
        if ((*componentIt)->GetType() == T::GetStaticType())
        {
            components.push_back(*componentIt);
        }
    }
    
    return components;
}
    
}
