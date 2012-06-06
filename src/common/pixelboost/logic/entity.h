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
    Uid GetUid();
    virtual Uid GetType();

    void Destroy();
    EntityState GetState();

public:
    typedef std::vector<Component*> ComponentList;
    typedef sigslot::Delegate2<Uid, Message&> MessageHandler;
    
public:
    Uid AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void RemoveAllComponents();
    
    template <class T>T* GetComponentByType();
    ComponentList* GetComponentsByType(Uid componentType);
    
    void SendMessage(Message& message);
    
    void RegisterMessageHandler(Uid messageType, MessageHandler handler);
    void UnregisterMessageHandler(Uid messageType, MessageHandler handler);
    
private:
    void HandleMessage(Message& message);
    
private:
    typedef std::map<Uid, sigslot::Signal2<Uid, Message&> > MessageHandlers;
    typedef std::map<Uid, ComponentList> ComponentMap;
    
    ComponentMap _Components;
    MessageHandlers _MessageHandlers;
    
    Scene* _Scene;
    Uid _Uid;
    
    EntityState _State;
};
    
}

#include "pixelboost/logic/entity.inl"
