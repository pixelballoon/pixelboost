#include "pixelboost/db/entity.h"
#include "pixelboost/logic/message/destroy.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

Entity::Entity(Scene* scene, DbEntity* entity)
    : _NextFreeUid(1)
    , _Scene(scene)
    , _Uid(0)
    , _State(kEntityCreated)
    , _HandlingMessage(false)
{
    _CreationUid = 0;
    _CreationEntity = entity;
    if (_CreationEntity)
    {
        _CreationUid = _CreationEntity->GetUid();
        _CreationEntity->structDestroyed.Connect(this, &Entity::HandleCreationEntityDestroyed);
        _CreationEntity->structReloaded.Connect(this, &Entity::OnCreationEntityReloaded);
    }
    
    _Uid = _Scene->GenerateEntityId();
    
    _Scene->AddEntity(this);
}

Entity::~Entity()
{
    if (_CreationEntity)
    {
        _CreationEntity->structDestroyed.Disconnect(this, &Entity::OnCreationEntityDestroyed);
        _CreationEntity->structReloaded.Disconnect(this, &Entity::OnCreationEntityReloaded);
    }
    
    for (ComponentList::iterator componentIt = _Components.begin(); componentIt != _Components.end(); ++componentIt)
    {
        delete *componentIt;
    }
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
    _Components.push_back(component);
}

void Entity::DestroyComponent(Component* component)
{
    if (component)
    {
        component->_State = Component::kComponentDestroyed;
        _Scene->AddEntityPurge(this);
    }
}

void Entity::PurgeComponents()
{
    for (ComponentList::iterator componentIt = _Components.begin(); componentIt != _Components.end();)
    {
        Component* component = *componentIt;
        if (component->_State == Component::kComponentDestroyed)
        {
            delete component;
            componentIt = _Components.erase(componentIt);
        } else {
            ++componentIt;
        }
    }
}

Component* Entity::GetComponentById(Uid componentId)
{
    for (ComponentList::iterator componentIt = _Components.begin(); componentIt != _Components.end(); ++componentIt)
    {
        if ((*componentIt)->GetUid() == componentId)
            return *componentIt;
    }
    
    return 0;
}

void Entity::CleanupMessageHandlers()
{
    for (std::map<Uid, std::vector<MessageHandler> >::iterator typeIt = _MessageHandlersCleanup.begin(); typeIt != _MessageHandlersCleanup.end(); ++typeIt)
    {
        MessageSignal* signal = 0;
        if (typeIt->first == 0)
        {
            signal = &_GenericMessageHandlers;
        } else {
            signal = &_MessageHandlers[typeIt->first];
        }
        
        for (std::vector<MessageHandler>::iterator handlerIt = typeIt->second.begin(); handlerIt != typeIt->second.end(); ++handlerIt)
        {
            signal->Disconnect(*handlerIt);
        }
    }
    
    _MessageHandlersCleanup.clear();
}

void Entity::RegisterMessageHandler(MessageHandler handler)
{
    _GenericMessageHandlers.Connect(handler);
}

void Entity::UnregisterMessageHandler(MessageHandler handler)
{
    if (_HandlingMessage)
    {
        _MessageHandlersCleanup[0].push_back(handler);
    } else {
        _GenericMessageHandlers.Disconnect(handler);
    }
}

void Entity::HandleMessage(const Message& message)
{
    _HandlingMessage = true;
    
    MessageHandlers::iterator handlerList = _MessageHandlers.find(message.GetType());
    
    if (handlerList != _MessageHandlers.end())
    {
        handlerList->second(message);
    }
    
    _GenericMessageHandlers(message);
    
    if (message.GetType() == DestroyMessage::GetStaticType())
        Destroy();
    
    _HandlingMessage = false;
    
    CleanupMessageHandlers();
}

void Entity::OnCreationEntityDestroyed()
{
    
}

void Entity::OnCreationEntityReloaded()
{
    
}

void Entity::HandleCreationEntityDestroyed()
{    
    _CreationEntity = 0;
    
    OnCreationEntityDestroyed();
}

void Entity::HandleCreationEntityReloaded()
{
    OnCreationEntityReloaded();
}
