#pragma once

#include <map>
#include <set>

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
    ~Entity();
    
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    
    void SendMessage(Message* message);
    
    void RegisterMessageHandler(Uid messageType, sigslot::Delegate2<Uid, Message*> handler);
    
private:
    typedef std::map<Uid, sigslot::Signal2<Uid, Message*> > MessageHandlers;
    
    typedef std::set<Component*> ComponentSet;
    
    ComponentSet _Components;
    MessageHandlers _MessageHandlers;
    
    Uid _Uid;
};
    
}
