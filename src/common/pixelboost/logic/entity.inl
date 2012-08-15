#include "pixelboost/db/entity.h"

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
    Entity::ComponentList components = GetComponentsByType(T::GetStaticType());
    
    if (components.size())
    {
        return static_cast<T*>(components.at(0));
    }
    
    return 0;
}
    
}
