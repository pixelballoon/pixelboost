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
    for (ComponentList::iterator componentIt = _Components.begin(); componentIt != _Components.end(); ++componentIt)
    {
        if ((*componentIt)->GetType() == T::GetStaticType())
        {
            return static_cast<T*>(*componentIt);
        }
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

template <class T> void Entity::RegisterMessageHandler(MessageHandler handler)
{
    _MessageHandlers[T::GetStaticType()].Connect(handler);
}

template <class T> void Entity::UnregisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlersCleanup[T::GetStaticType()].push_back(handler);
    } else {
        _MessageHandlers[T::GetStaticType()].Disconnect(handler);
    }
}
    
}
