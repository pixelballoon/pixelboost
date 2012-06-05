#pragma once

#include <map>
#include <vector>

#include "sigslot/signal.h"

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Component;
class Message;

class Entity
{
public:
    Entity(Uid uid);
    virtual ~Entity();
    
public:
    enum EntityState
    {
        kEntityCreated,
        kEntityKilled
    };
    
public:
    Uid GetUid();
    virtual Uid GetType();

    EntityState GetState();

public:
    typedef std::vector<Component*> ComponentList;
    typedef sigslot::Delegate2<Uid, Message&> MessageHandler;
    
public:
    Uid AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void RemoveAllComponents();
    
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
    
    EntityState _State;
    Uid _Uid;
};
    
}
