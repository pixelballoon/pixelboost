#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"

using namespace pb;

Entity::Entity(Uid uid)
    : _Uid(uid)
{
    
}

Entity::~Entity()
{
    
}

Uid Entity::GetUid()
{
    return _Uid;
}

Uid Entity::GetType()
{
    return 0;
}

Uid Entity::AddComponent(Component* component)
{
    _Components[component->GetType()].push_back(component);
    
    return 0;
}

void Entity::RemoveComponent(Component* component)
{
    ComponentMap::iterator groupIt = _Components.find(component->GetType());
    
    if (groupIt != _Components.end())
    {
        for (ComponentList::iterator componentIt = groupIt->second.begin(); componentIt != groupIt->second.end(); ++componentIt)
        {
            if (*componentIt == component)
            {
                groupIt->second.erase(componentIt);
                delete component;
                return;
            }
        }
    }
}

Entity::ComponentList* Entity::GetComponentsByType(Uid componentType)
{
    ComponentMap::iterator it = _Components.find(componentType);
    
    if (it != _Components.end())
        return &it->second;
    
    return 0;
}

void Entity::SendMessage(Message& message)
{
    MessageHandlers::iterator handlerList = _MessageHandlers.find(message.GetType());
    
    if (handlerList != _MessageHandlers.end())
    {
        handlerList->second(_Uid, message);
    }
}

void Entity::RegisterMessageHandler(Uid messageType, sigslot::Delegate2<Uid, Message&> handler)
{
    _MessageHandlers[messageType].Connect(handler);
}

void Entity::UnregisterMessageHandler(Uid messageType, sigslot::Delegate2<Uid, Message&> handler)
{
    _MessageHandlers[messageType].Disconnect(handler);
}