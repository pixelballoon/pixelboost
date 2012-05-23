#include "project/entity.h"
#include "project/record.h"
#include "view/entity/entity.h"
#include "view/level.h"

using namespace pixeleditor;

Level::Level()
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
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        delete it->second;
    }
    _Entities.clear();
}

void Level::SetRecord(Record* record)
{
    Clear();
    
    for (Record::EntityMap::const_iterator it = record->GetEntities().begin(); it != record->GetEntities().end(); ++it)
    {
        Entity* entity = it->second;
        Uid id = GenerateViewEntityId(entity);
        ViewEntity* viewEntity = new ViewEntity(id, entity);
        _Entities[id] = viewEntity;
    }
}

Uid Level::GenerateViewEntityId(Entity* entity)
{
    Uid uid;
    
    do {
        uid = rand()%(1<<12);
    } while (_EntityIdMap.find(uid) != _EntityIdMap.end());
    
    _EntityIdMap[entity->GetUid()] = uid;
    
    return uid;
}

ViewEntity* Level::GetEntityByEntityId(Uid uid)
{
    EntityIdMap::iterator it = _EntityIdMap.find(uid);
    
    if (it != _EntityIdMap.end())
    {
        return GetEntityByViewEntityId(uid);
    }
    
    return 0;
}

ViewEntity* Level::GetEntityByViewEntityId(Uid uid)
{
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
    {
        return it->second;
    }
    
    return 0;
}
