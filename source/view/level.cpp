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
        ViewEntity* viewEntity = new ViewEntity(entity);
        _Entities[entity->GetUid()] = viewEntity;
    }
}
