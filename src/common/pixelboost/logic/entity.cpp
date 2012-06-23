#include "pixelboost/logic/message/destroy.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Entity::Entity(Scene* scene, Uid uid)
    : _NextFreeUid(1)
    , _Scene(scene)
    , _Uid(uid)
    , _State(kEntityCreated)
{
    if (_Uid == 0)
    {
        _Uid = _Scene->GenerateEntityId();
    }
}

Entity::~Entity()
{
    DestroyAllComponents();
}

Scene* Entity::GetScene()
{
    return _Scene;
}

Uid Entity::GetUid()
{
    return _Uid;
}

Uid Entity::GetType()
{
    return 0;
}

void Entity::Destroy()
{
    _State = kEntityDestroyed;
}

Entity::EntityState Entity::GetState()
{
    return _State;
}

Uid Entity::GenerateComponentId()
{
    return _NextFreeUid++;
}

void Entity::AddComponent(Component* component)
{
    _Components[component->GetType()].push_back(component);
}

void Entity::DestroyComponent(Component* component)
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

void Entity::DestroyAllComponents()
{
    for (ComponentMap::iterator groupIt = _Components.begin(); groupIt != _Components.end(); ++groupIt)
    {
        for (ComponentList::iterator componentIt = groupIt->second.begin(); componentIt != groupIt->second.end(); ++componentIt)
        {
            delete *componentIt;
        }
    }
    
    _Components.clear();
}

Component* Entity::GetComponentById(Uid componentId)
{
    for (ComponentMap::iterator groupIt = _Components.begin(); groupIt != _Components.end(); ++groupIt)
    {
        for (ComponentList::iterator componentIt = groupIt->second.begin(); componentIt != groupIt->second.end(); ++componentIt)
        {
            if ((*componentIt)->GetUid() == componentId)
                return *componentIt;
        }
    }
    
    return 0;
}

Entity::ComponentList* Entity::GetComponentsByType(Uid componentType)
{
    ComponentMap::iterator it = _Components.find(componentType);
    
    if (it != _Components.end())
        return &it->second;
    
    return 0;
}

void Entity::RegisterMessageHandler(Uid messageType, sigslot::Delegate2<Uid, const Message&> handler)
{
    _MessageHandlers[messageType].Connect(handler);
}

void Entity::UnregisterMessageHandler(Uid messageType, sigslot::Delegate2<Uid, const Message&> handler)
{
    _MessageHandlers[messageType].Disconnect(handler);
}

void Entity::HandleMessage(const Message& message)
{
    MessageHandlers::iterator handlerList = _MessageHandlers.find(message.GetType());
    
    if (handlerList != _MessageHandlers.end())
    {
        handlerList->second(_Uid, message);
    }
    
    if (message.GetType() == DestroyMessage::GetStaticType())
        Destroy();
}
