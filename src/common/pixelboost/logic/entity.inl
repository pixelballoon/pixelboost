#include "pixelboost/db/entity.h"
#include "pixelboost/debug/assert.h"
#include "pixelboost/logic/component.h"

namespace pb
{

template<class T> const T* Entity::GetData() const
{
    if (!_CreationEntity)
        return 0;
        
    return _CreationEntity->GetData<T>();
}

template <class T>T* Entity::CreateComponent()
{
    PbAssert(_Components.find(T::GetStaticType()) == _Components.end() || _Components[T::GetStaticType()] == 0);

    T* component = static_cast<T*>(T::Create(this));
    _Components[component->GetType()] = component;
    return component;
}

template <class T>T* Entity::GetComponent()
{
    for (const auto& component : _Components)
    {
        if (component.second && component.second->IsA(T::GetStaticType()))
        {
            return static_cast<T*>(component.second);
        }
    }

    return 0;
}

template <class T> void Entity::RegisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlerDelayedAdd[T::GetStaticType()].insert(handler);
        _MessageHandlerDelayedRemove[T::GetStaticType()].erase(handler);
    } else {
        _MessageHandlers[T::GetStaticType()].Connect(handler);
    }
}

template <class T> void Entity::UnregisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlerDelayedAdd[T::GetStaticType()].erase(handler);
        _MessageHandlerDelayedRemove[T::GetStaticType()].insert(handler);
    } else {
        _MessageHandlers[T::GetStaticType()].Disconnect(handler);
    }
}
    
}
