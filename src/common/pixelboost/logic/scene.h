#pragma once

#include <set>

namespace pb
{
    
class Entity;
class Viewport;
    
class Scene
{
public:
    Scene();
    ~Scene();
    
    void Update(float time);
    void Render(Viewport* viewport);
    
    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);
    
private:
    typedef std::set<Entity*> EntitySet;
    
    EntitySet _Entities;
};
    
}
