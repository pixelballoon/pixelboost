#pragma once

#include <map>
#include <vector>

#include "sigslot/signal.h"

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Component;
class DbEntity;
class Message;
class Scene;

class Entity
{
public:
    Entity(Scene* scene, DbEntity* creationEntity);
    virtual ~Entity();
    
public:
    enum EntityState
    {
        kEntityCreated,
        kEntityDestroyed
    };
    
public:
    Scene* GetScene();
    
    Uid GetCreationUid();
    
    Uid GetUid();
    virtual Uid GetType();
    
    template<class T> const T* GetData() const;

    void Destroy();
    EntityState GetState();

public:
    typedef std::vector<Component*> ComponentList;
    typedef sigslot::Delegate2<Uid, const Message&> MessageHandler;
    
public:
    Uid GenerateComponentId();
    
    void AddComponent(Component* component);
    void DestroyComponent(Component* component);
    void DestroyAllComponents();
    
    Component* GetComponentById(Uid componentId);
    
    template <class T>T* GetComponentByType();
    ComponentList* GetComponentsByType(Uid componentType);
    
    void RegisterMessageHandler(Uid messageType, MessageHandler handler);
    void UnregisterMessageHandler(Uid messageType, MessageHandler handler);
    
    void HandleMessage(const Message& message);
    
    virtual void HandleCreationEntityDestroyed();
    
private:
    void OnCreationEntityDestroyed();
    
    typedef std::map<Uid, sigslot::Signal2<Uid, const Message&> > MessageHandlers;
    typedef std::map<Uid, ComponentList> ComponentMap;
    
    ComponentMap _Components;
    MessageHandlers _MessageHandlers;
    
    Scene* _Scene;
    
    DbEntity* _CreationEntity;
    Uid _CreationUid;
    
    Uid _Uid;
    
    Uid _NextFreeUid;
    
    EntityState _State;
};
    
}

#include "pixelboost/logic/entity.inl"
