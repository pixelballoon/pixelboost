#pragma once

#include <map>

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Entity;
class Message;
class SceneSystem;
class Viewport;
    
class Scene
{
public:
    Scene();
    ~Scene();
    
    void Update(float time);
    void Render(Viewport* viewport);
    
    template <class T>T* GetSystemByType();
    bool AddSystem(SceneSystem* system);
    void RemoveSystem(SceneSystem* system);
    
    pb::Uid GenerateEntityId();
    
    void AddEntity(Entity* entity);
    void DestroyEntity(Entity* entity);
    void DestroyAllEntities();
    
    Entity* GetEntityById(Uid uid);
    
    void BroadcastMessage(const Message& message);
    void SendMessage(Uid uid, const Message& message);
    void SendDelayedMessage(Uid uid, const Message& message);
    
private:
    typedef std::map<Uid, Entity*> EntityMap;
    typedef std::map<Uid, SceneSystem*> SystemMap;
    
    EntityMap _NewEntities;
    EntityMap _Entities;
    SystemMap _Systems;
    
    pb::Uid _NextFreeUid;
};
    
}

#include "pixelboost/logic/scene.inl"
