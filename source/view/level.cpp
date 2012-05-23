#include "project/entity.h"
#include "project/record.h"
#include "view/entity/entity.h"
#include "view/level.h"

using namespace pixeleditor;

Level::Level()
    : _Record(0)
{
    
}

Level::~Level()
{
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

