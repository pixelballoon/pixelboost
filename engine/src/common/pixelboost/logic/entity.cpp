#include "pixelboost/db/entity.h"
#include "pixelboost/debug/assert.h"
#include "pixelboost/logic/message/destroy.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

Entity::Entity(Scene* scene, Entity* parent, DbEntity* entity)
    : _Scene(scene)
    , _Parent(parent)
    , _Uid(0)
    , _State(kEntityCreated)
    , _HandlingMessage(0)
{
    _CreationEntity = entity;
    if (_CreationEntity)
    {
        _CreationEntity->structDestroyed.Connect(this, &Entity::HandleCreationEntityDestroyed);
        _CreationEntity->structReloaded.Connect(this, &Entity::HandleCreationEntityReloaded);
    }
    
    _Uid = _Scene->GenerateEntityId();
    
    if (parent)
    {
        parent->AddChild(this);
    }
}

Entity::~Entity()
{
    if (_Parent)
    {
        _Parent->RemoveChild(this);
    }
    
    if (_CreationEntity)
    {
        _CreationEntity->structDestroyed.Disconnect(this, &Entity::HandleCreationEntityDestroyed);
        _CreationEntity->structReloaded.Disconnect(this, &Entity::HandleCreationEntityReloaded);
    }
    
    for (const auto& child : _Children)
    {
        child->_Parent = 0;
    }
    
    for (const auto& component : _Components)
    {
        delete component.second;
    }
    
    for (const auto& component : _PendingComponents)
    {
        delete component;
    }
}

Entity* Entity::Create(Scene* scene, Entity* parent, DbEntity* creationEntity)
{
    return new Entity(scene, parent, creationEntity);
}

void Entity::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .beginClass<Entity>("Entity")
            .addFunction("GetUid", &Entity::GetUid)
        .endClass()
    .endNamespace();
}

Scene* Entity::GetScene()
{
    return _Scene;
}

Entity* Entity::GetParent()
{
    return _Parent;
}

Uid Entity::GetCreationUid()
{
    if (_CreationEntity)
    {
        return _CreationEntity->GetUid();
    }
    
    return 0;
}

Uid Entity::GetUid()
{
    return _Uid;
}

const std::string& Entity::GetName()
{
    return _Name;
}

void Entity::SetName(const std::string& name)
{
    _Name = name;
}

Uid Entity::GetType() const
{
    return GetStaticType();
}

pb::Uid Entity::GetStaticType()
{
    return pb::TypeHash("pb::Entity");
}

const DbEntity* Entity::GetCreationEntity() const
{
    return _CreationEntity;
}

void Entity::Destroy()
{
    if (_State != kEntityDestroyed)
    {
        for (const auto& child : _Children)
        {
            child->_Parent = 0;
            child->Destroy();
        }
        _Children.clear();
        
        _State = kEntityDestroyed;
    }
}

Entity::EntityState Entity::GetState()
{
    return _State;
}

const std::set<Entity*>& Entity::GetChildren()
{
    return _Children;
}

Entity* Entity::FindChildByName(const std::string& name)
{
    for (const auto& child : _Children)
    {
        if (child->GetName() == name)
            return child;
    }
    
    return 0;
}

void Entity::DestroyComponent(Component* component)
{
    if (component)
    {
        _PendingComponents.insert(component);
        _Components[component->GetType()] = 0;
        component->_State = Component::kComponentDestroyed;
        _Scene->AddEntityPurge(this);
    }
}

void Entity::DestroyAllComponents()
{
    if (!_Components.size())
        return;
    
    for (auto& component : _Components)
    {
        if (component.second)
        {
            _PendingComponents.insert(component.second);
            component.second->_State = Component::kComponentDestroyed;
            component.second = 0;
        }
    }
    
    _Scene->AddEntityPurge(this);
}

std::vector<Component*> Entity::GetComponents()
{
    std::vector<Component*> components;
    for (const auto& component : _Components)
    {
        components.push_back(component.second);
    }
    return components;
}

void Entity::RegisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlerDelayedAdd[0].insert(handler);
        _MessageHandlerDelayedRemove[0].erase(handler);
    } else {
        _MessageHandlers[0].Connect(handler);
    }
}

void Entity::UnregisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlerDelayedAdd[0].erase(handler);
        _MessageHandlerDelayedRemove[0].insert(handler);
    } else {
        _MessageHandlers[0].Disconnect(handler);
    }
}

void Entity::AddChild(Entity* child)
{
    _Children.insert(child);
}

void Entity::RemoveChild(Entity* child)
{
    _Children.erase(child);
}

void Entity::SendMessage(const Message& message, bool sendToSelf, bool broadcastParents, bool broadcastChildren)
{
    if (sendToSelf)
    {
        HandleMessage(message);
    }
    
    if (broadcastParents)
    {
        if (_Parent)
        {
            _Parent->SendMessage(message, true, true, false);
        }
    }
    
    if (broadcastChildren)
    {
        for (const auto& child : _Children)
        {
            child->SendMessage(message, true, false, true);
        }
    }
}

void Entity::HandleMessage(const Message& message)
{
    _HandlingMessage++;
    
    MessageHandlers::iterator handlerList = _MessageHandlers.find(message.GetType());
    
    if (handlerList != _MessageHandlers.end())
    {
        handlerList->second(message);
    }
    
    _MessageHandlers[0](message);
    
    if (message.GetType() == DestroyMessage::GetStaticType())
        Destroy();
    
    _HandlingMessage--;
    
    SyncMessageHandlers();
}


void Entity::Purge()
{
    for (auto& component : _PendingComponents)
    {
        delete component;
    }
    
    _PendingComponents.clear();
}

void Entity::SyncMessageHandlers()
{
    for (const auto& type : _MessageHandlerDelayedAdd)
    {
        for (const auto& handler : type.second)
        {
            _MessageHandlers[type.first].Connect(handler);
        }
    }
    
    for (const auto& type : _MessageHandlerDelayedRemove)
    {
        for (const auto& handler : type.second)
        {
            _MessageHandlers[type.first].Disconnect(handler);
        }
    }
    
    _MessageHandlerDelayedAdd.clear();
    _MessageHandlerDelayedRemove.clear();
}

void Entity::HandleCreationEntityDestroyed()
{    
    _CreationEntity = 0;
    
    // TODO : Send notification message
}

void Entity::HandleCreationEntityReloaded()
{
    // TODO : Send notification message
}
