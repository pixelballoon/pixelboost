#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Scene::Scene()
{
    
}

Scene::~Scene()
{
    
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
    
    UpdateMessage message(time);
    
    SendMessage(message);
}

void Scene::Render(Viewport* viewport)
{
    RenderMessage message(viewport);
    
    SendMessage(message);
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
