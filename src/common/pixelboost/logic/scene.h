#pragma once

#include <map>
#include <set>
#include <vector>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/definitions.h"
#include "pixelboost/logic/definitions.h"

struct lua_State;

namespace pb
{
    
class DbEntity;
class DelayedMessage;
class Entity;
class Message;
class SceneSystem;
class Viewport;
    
class Scene
{
public:
    Scene();
    ~Scene();
    
    static void RegisterLuaClass(lua_State* state);
    
    void Update(float timeDelta, float gameDelta);
    void Render(Viewport* viewport, RenderPass renderPass);
    
    template <class T>T* GetSystemByType();
    bool AddSystem(SceneSystem* system);
    void RemoveSystem(SceneSystem* system);
    
public:
    typedef std::map<Uid, Entity*> EntityMap;
    
    pb::Uid GenerateEntityId();
    
    template <class T>T* CreateEntity(Entity* parent, DbEntity* creationEntity);
    void DestroyEntity(Entity* entity);
    void DestroyAllEntities();
    
    Entity* GetEntityById(Uid uid);
    const EntityMap& GetEntities() const;
    template <class T> EntityMap GetEntitiesByType();
    
public:
    void BroadcastMessage(const Message& message);
    void SendMessage(Uid uid, const Message& message);
    
private:
    void AddEntity(Entity* entity);
    void AddEntityPurge(Entity* entity);
    
    typedef std::set<Entity*> EntitySet;
    typedef std::map<Uid, SceneSystem*> SystemMap;
    
    EntityMap _NewEntities;
    EntityMap _Entities;
    SystemMap _Systems;
    
    EntitySet _PurgeSet;
    
    pb::Uid _NextFreeUid;
    
    friend class Entity;
};
    
}

#include "pixelboost/logic/scene.inl"
