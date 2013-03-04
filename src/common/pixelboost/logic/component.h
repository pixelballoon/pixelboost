#pragma once

#include "pixelboost/framework/definitions.h"

struct lua_State;

#define PB_DECLARE_COMPONENT public: virtual bool IsA(pb::Uid type) const; virtual pb::Uid GetType() const; static pb::Uid GetStaticType();
#define PB_DEFINE_COMPONENT_DERIVED(className, parent) bool className::IsA(pb::Uid type) const { return GetStaticType() == type || parent::IsA(type); } pb::Uid className::GetType() const { return GetStaticType(); }  pb::Uid className::GetStaticType() { return pb::TypeHash(#className); }
#define PB_DEFINE_COMPONENT(className) PB_DEFINE_COMPONENT_DERIVED(className, pb::Component)

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
    
    virtual bool IsA(Uid type) const;
    virtual Uid GetType() const;
    static Uid GetStaticType();
    
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
