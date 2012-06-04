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
    UpdateMessage message(0, time);
    
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        (*it)->SendMessage(message);
    }
}

void Scene::Render(Viewport* viewport)
{
    RenderMessage message(0, viewport);
    
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        (*it)->SendMessage(message);
    }
}

void Scene::AddEntity(Entity* entity)
{
    _Entities.insert(entity);
}

void Scene::RemoveEntity(Entity* entity)
{
    _Entities.erase(entity);
}
