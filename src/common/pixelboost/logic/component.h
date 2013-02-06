#pragma once

#include "pixelboost/db/definitions.h"

struct lua_State;

namespace pb
{
    
class Entity;
class Scene;

class Component
{
public:
    Component(Entity* parent);
    
    static void RegisterLuaClass(lua_State* state);
    
protected:
    virtual ~Component();
    
public:
    enum ComponentState
    {
        kComponentCreated,
        kComponentDestroyed
    };
    
    virtual Uid GetType() = 0;
    
    Uid GetUid();
    
    Scene* GetScene();
    Entity* GetParent();
    pb::Uid GetParentUid();
    
private:
    Entity* _Parent;
    Uid _Uid;
    
    ComponentState _State;
    
    friend class Entity;
};

}
