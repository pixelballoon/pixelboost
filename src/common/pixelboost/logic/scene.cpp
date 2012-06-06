#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/logic/system.h"

using namespace pb;

Scene::Scene()
{
    
}

Scene::~Scene()
{
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        delete *it;
    }
}

void Scene::Update(float time)
{
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end();)
    {
        if ((*it)->GetState() == Entity::kEntityDestroyed)
        {
            delete *it;
            _Entities.erase(it++);
        } else {
            ++it;
        }
    }
    
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
    {
        it->second->Update(this, time);
    }
    
    UpdateMessage message(time);
    
    SendMessage(message);
}

void Scene::Render(Viewport* viewport)
{
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
    {
        it->second->Render(this, viewport);
    }
}

bool Scene::AddSystem(SceneSystem* system)
{
    SystemMap::iterator it = _Systems.find(system->GetType());
    
    if (it != _Systems.end())
        return false;
        
    _Systems[system->GetType()] = system;
    
    return true;
}

void Scene::RemoveSystem(SceneSystem* system)
{
    SystemMap::iterator it = _Systems.find(system->GetType());
    
    if (it != _Systems.end())
    {
        _Systems.erase(it);
    }
}

void Scene::AddEntity(Entity* entity)
{
    _Entities.insert(entity);
}

void Scene::DestroyEntity(Entity* entity)
{
    entity->Destroy();
}

void Scene::DestroyAllEntities()
{
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        (*it)->Destroy();
    }
}

void Scene::SendMessage(Message& message)
{
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        (*it)->SendMessage(message);
    }
}
