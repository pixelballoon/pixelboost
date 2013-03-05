#pragma once

#include "pixelboost/framework/definitions.h"

struct lua_State;

#define PB_DECLARE_COMPONENT public: static pb::Component* Create(pb::Entity* entity); virtual bool IsA(pb::Uid type) const; virtual pb::Uid GetType() const; static pb::Uid GetStaticType();
#define PB_DEFINE_COMPONENT_DERIVED_ABSTRACT(className, parent) bool className::IsA(pb::Uid type) const { return GetStaticType() == type || parent::IsA(type); } pb::Uid className::GetType() const { return GetStaticType(); }  pb::Uid className::GetStaticType() { return pb::TypeHash(#className); }
#define PB_DEFINE_COMPONENT_DERIVED(className, parent) pb::Component* className::Create(pb::Entity* entity) { return new className(entity); } PB_DEFINE_COMPONENT_DERIVED_ABSTRACT(className, parent)
#define PB_DEFINE_COMPONENT(className) PB_DEFINE_COMPONENT_DERIVED(className, pb::Component)
#define PB_DEFINE_COMPONENT_ABSTRACT(className) PB_DEFINE_COMPONENT_DERIVED_ABSTRACT(className, pb::Component)

namespace pb
{
    
class Entity;
class Scene;

class Component
{
protected:
    Component(Entity* entity);
    virtual ~Component();
    
public:
    static void RegisterLuaClass(lua_State* state);
    
public:
    enum ComponentState
    {
        kComponentCreated,
        kComponentDestroyed
    };
    
    virtual bool IsA(Uid type) const;
    virtual Uid GetType() const;
    static Uid GetStaticType();
    
    Scene* GetScene();
    Entity* GetEntity();
    pb::Uid GetEntityUid();
    
private:
    Entity* _Parent;
    
    ComponentState _State;
    
    friend class Entity;
};

}
