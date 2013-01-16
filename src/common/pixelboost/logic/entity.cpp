#include "pixelboost/db/entity.h"
#include "pixelboost/logic/message/destroy.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Entity::Entity(Scene* scene, DbEntity* entity)
    : _NextFreeUid(1)
    , _Scene(scene)
    , _Uid(0)
    , _State(kEntityCreated)
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

void Entity::HandleMessage(const Message& message)
{
    MessageHandlers::iterator handlerList = _MessageHandlers.find(message.GetType());
    
    if (handlerList != _MessageHandlers.end())
    {
        handlerList->second(message);
    }
    
    if (message.GetType() == DestroyMessage::GetStaticType())
        Destroy();
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
