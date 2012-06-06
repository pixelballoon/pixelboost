#pragma once

#include <set>

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
    
    void AddEntity(Entity* entity);
    void DestroyEntity(Entity* entity);
    void DestroyAllEntities();
    
    void SendMessage(Message& message);
    
private:
    typedef std::set<Entity*> EntitySet;
    typedef std::map<Uid, SceneSystem*> SystemMap;
    
    EntitySet _Entities;
    SystemMap _Systems;
};
    
}

#include "pixelboost/logic/scene.inl"
