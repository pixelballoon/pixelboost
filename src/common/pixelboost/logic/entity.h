#pragma once

#include <map>
#include <vector>

#include "sigslot/signal.h"

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Component;
class Message;
class Scene;

class Entity
{
public:
    Entity(Scene* scene, Uid uid);
    virtual ~Entity();
    
public:
    enum EntityState
    {
        kEntityCreated,
        kEntityDestroyed
    };
    
public:
    Scene* GetScene();
    
    Uid GetUid();
    virtual Uid GetType();

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
    
    void SendMessage(const Message& message);
    
    void RegisterMessageHandler(Uid messageType, MessageHandler handler);
    void UnregisterMessageHandler(Uid messageType, MessageHandler handler);
    
private:
    void HandleMessage(const Message& message);
    
private:
    typedef std::map<Uid, sigslot::Signal2<Uid, const Message&> > MessageHandlers;
    typedef std::map<Uid, ComponentList> ComponentMap;
    
    ComponentMap _Components;
    MessageHandlers _MessageHandlers;
    
    Scene* _Scene;
    Uid _Uid;
    
    Uid _NextFreeUid;
    
    EntityState _State;
};
    
}

#include "pixelboost/logic/entity.inl"
