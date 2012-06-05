#pragma once

#include <set>

namespace pb
{
    
class Entity;
class Message;
class Viewport;
    
class Scene
{
public:
    Scene();
    ~Scene();
    
    void Update(float time);
    void Render(Viewport* viewport);
    
    void AddEntity(Entity* entity);
    void DestroyEntity(Entity* entity);
    void DestroyAllEntities();
    
    void SendMessage(Message& message);
    
private:
    typedef std::set<Entity*> EntitySet;
    
    EntitySet _Entities;
};
    
}
