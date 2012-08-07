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
    
    for (DelayedMessageList::iterator it = _DelayedMessages.begin(); it != _DelayedMessages.end(); ++it)
    {
        delete it->second.second;
    }
    
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
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
    
    for (EntityMap::iterator it = _NewEntities.begin(); it != _NewEntities.end(); ++it)
    {
#ifndef PIXELBOOST_DISABLE_DEBUG
        PbAssert(_Entities.find(it->first) == _Entities.end());
#endif
        _Entities[it->first] = it->second;
    }
    _NewEntities.clear();
    
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
    {
        it->second->Update(this, time);
    }
    
    for (DelayedMessageList::iterator it = _DelayedMessages.begin(); it != _DelayedMessages.end();)
    {
        it->first -= time;
        
        if (it->first < 0)
        {
            if (it->second.first == 0)
            {
                BroadcastMessage(*it->second.second);
            } else {
                SendMessage(it->second.first, *it->second.second);
            }
            delete it->second.second;
            it = _DelayedMessages.erase(it);
        } else {
            ++it;
        }
    }
    
    UpdateMessage message(time);
    
    BroadcastMessage(message);
}

void Scene::Render(Viewport* viewport, RenderPass renderPass)
{
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
    {
        it->second->Render(this, viewport, renderPass);
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
    PbAssert(_NewEntities.find(entity->GetUid()) == _NewEntities.end());
#endif
    _NewEntities[entity->GetUid()] = entity;
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
    
    it = _NewEntities.find(uid);
    
    if (it != _NewEntities.end())
        return it->second;
    
    return 0;
}

const Scene::EntityMap& Scene::GetEntities() const
{
    return _Entities;
}

void Scene::BroadcastMessage(const Message& message)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetState() != Entity::kEntityDestroyed)
            it->second->HandleMessage(message);
    }
    
    for (SystemMap::iterator it = _Systems.begin(); it != _Systems.end(); ++it)
    {
        it->second->HandleMessage(this, message);
    }
}

void Scene::SendMessage(Uid uid, const Message& message)
{
    Entity* entity = GetEntityById(uid);
    
    if (entity && entity->GetState() != Entity::kEntityDestroyed)
        entity->HandleMessage(message);
}

void Scene::BroadcastDelayedMessage(float delay, const Message* message)
{
    _DelayedMessages.push_back(std::pair<float, DelayedMessage>(delay, DelayedMessage(0, message)));
}

void Scene::SendDelayedMessage(Uid uid, float delay, const Message* message)
{
    _DelayedMessages.push_back(std::pair<float, DelayedMessage>(delay, DelayedMessage(uid, message)));
}
