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

void Scene::RemoveEntity(Entity* entity)
{
    _Entities.erase(entity);
}

void Scene::SendMessage(Message& message)
{
    for (EntitySet::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        (*it)->SendMessage(message);
    }
}
