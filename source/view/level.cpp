#include "pixelboost/graphics/camera/camera.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/record.h"
#include "view/entity/entity.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

Level::Level()
    : _Record(0)
{
    View::Instance()->GetMouseManager()->AddHandler(this);
}

Level::~Level()
{
    View::Instance()->GetMouseManager()->RemoveHandler(this);
    
    Clear();
}

void Level::Update(float time)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->Update(time);
    }    
}

void Level::Render(pb::RenderLayer* layer)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->Render(layer);
    }
}

void Level::Clear()
{
    while (_Entities.size())
        DestroyEntity(_Entities.begin()->second->GetUid());
    
    _Record = 0;
}

void Level::SetRecord(Record* record)
{
    Clear();
    
    if (record)
    {
        _Record = record;
        
        for (Record::EntityMap::const_iterator it = record->GetEntities().begin(); it != record->GetEntities().end(); ++it)
        {
            Entity* entity = it->second;
            CreateEntity(entity);
        }
    }
}

ViewEntity* Level::GetEntityById(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
    {
        return it->second;
    }
    
    return 0;
}
                     
void Level::CreateEntity(Uid uid)
{
    Entity* entity = _Record->GetEntity(uid);

    if (entity)
        CreateEntity(entity);
}

void Level::CreateEntity(Entity* entity)
{
    ViewEntity* viewEntity = new ViewEntity(entity->GetUid(), entity);
    _Entities[entity->GetUid()] = viewEntity;
    entityAdded(viewEntity);
}

void Level::DestroyEntity(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
    {
        entityRemoved(it->second);
        delete it->second;
        _Entities.erase(it);
    }
}

int Level::GetPriority()
{
    return 0;
}

bool Level::OnMouseDown(pb::MouseButton button, glm::vec2 position)
{
    glm::vec2 worldPos = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(position);
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetBoundingBox().Contains(glm::vec3(worldPos[0], worldPos[1], 0)))
        {
            char args[256];
            sprintf(args, "-u %d", it->first);
            Core::Instance()->GetCommandManager()->Exec("select", args);
            return true;
        }
    }
    
    Core::Instance()->GetCommandManager()->Exec("select", "-c");
    
    return true;
}

bool Level::OnMouseUp(pb::MouseButton button, glm::vec2 position)
{
    return false;
}

bool Level::OnMouseMove(glm::vec2 position)
{
    return false;
}
