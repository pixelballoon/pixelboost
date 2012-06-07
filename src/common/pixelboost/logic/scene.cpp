#include "pixelboost/debug/assert.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/logic/system.h"

using namespace pb;

Scene::Scene()
{
    _NextFreeUid = (1<<24) + 1;
}

Scene::~Scene()
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        delete it->second;
    }
}

void Scene::Update(float time)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end();)
    {
        if (it->second->GetState() == Entity::kEntityDestroyed)
        {
            delete it->second;
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
    
    BroadcastMessage(message);
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

pb::Uid Scene::GenerateEntityId()
{
    return _NextFreeUid++;
}

void Scene::AddEntity(Entity* entity)
{
#ifndef PIXELBOOST_DISABLE_DEBUG
    PbAssert(_Entities.find(entity->GetUid()) == _Entities.end());
#endif
    _Entities[entity->GetUid()] = entity;
}

void Scene::DestroyEntity(Entity* entity)
{
    entity->Destroy();
}

void Scene::DestroyAllEntities()
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->Destroy();
    }
}

Entity* Scene::GetEntityById(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}

void Scene::BroadcastMessage(Message& message)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->SendMessage(message);
    }
}

void Scene::SendMessage(Uid uid, Message& message)
{
    Entity* entity = GetEntityById(uid);
    
    if (entity)
        entity->SendMessage(message);
}
