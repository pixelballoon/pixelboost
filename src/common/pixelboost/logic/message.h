#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{

class Component;
class Entity;
    
class Message
{
public:
    Message(Entity* entity, Component* component);
    virtual ~Message();
    
    virtual Uid GetType() = 0;
    
    Entity* GetEntity();
    Component* GetComponent();
    
private:
    Entity* _Entity;
    Component* _Component;
};
    
}
