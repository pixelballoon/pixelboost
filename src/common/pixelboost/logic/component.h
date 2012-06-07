#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Entity;
class Scene;

class Component
{
public:
    Component(Entity* parent);
    virtual ~Component();
    
    virtual Uid GetType() = 0;
    
    Uid GetUid();
    
    Scene* GetScene();
    Entity* GetParent();
    
private:
    Entity* _Parent;
    Uid _Uid;
};

}
