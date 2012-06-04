#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Entity;

class Component
{
public:
    Component(Entity* parent);
    virtual ~Component();
    
    virtual Uid GetType() = 0;
    
    Entity* GetParent();
    
private:
    Entity* _Parent;
};

}
