#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

Level::Level()
    : _Record(0)
{
    View::Instance()->GetMouseManager()->AddHandler(this);
    
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &Level::OnRecordRemoved);
    
    _Size = glm::vec2(0,0);
}

Level::~Level()
{
    Core::Instance()->GetProject()->recordRemoved.Disconnect(this, &Level::OnRecordRemoved);
    
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

void Level::Render(pb::RenderLayer* backgroundLayer, pb::RenderLayer* levelLayer)
{
    View::Instance()->GetPrimitiveRenderer()->AttachBox(backgroundLayer, Vec2(0,0), Vec2(_Size.x, _Size.y), Vec3(0,0,0), Vec4(0.6, 0.6, 0.6, 1.0), true);
    View::Instance()->GetPrimitiveRenderer()->AttachBox(backgroundLayer, Vec2(0,0), Vec2(_Size.x, _Size.y), Vec3(0,0,0), Vec4(0, 0, 0, 1.0), false);
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->Render(levelLayer);
    }
}

void Level::Clear()
{
    if (_Record)
    {
        _Record->propertyChanged.Disconnect(this, &Level::OnPropertyChanged);
        _Record->entityAdded.Disconnect(this, &Level::OnEntityAdded);
        _Record->entityRemoved.Disconnect(this, &Level::OnEntityRemoved);
    }
    
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
        
        UpdateSize();
        
        _Record->propertyChanged.Connect(this, &Level::OnPropertyChanged);
        _Record->entityAdded.Connect(this, &Level::OnEntityAdded);
        _Record->entityRemoved.Connect(this, &Level::OnEntityRemoved);
        
        for (Record::EntityMap::const_iterator it = record->GetEntities().begin(); it != record->GetEntities().end(); ++it)
        {
            Entity* entity = it->second;
            CreateEntity(entity);
        }
    }
}

const Level::EntityMap& Level::GetEntities()
{
    return _Entities;
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

void Level::UpdateSize()
{
    _Size = glm::vec2(0,0);
    
    if (!_Record)
        return;

    const SchemaAttribute* hasLevel = _Record->GetType()->GetAttribute("HasLevel");
    if (!hasLevel)
        return;
    
    std::string width = hasLevel->EvaluateParamValue(_Record, "width");
    std::string height = hasLevel->EvaluateParamValue(_Record, "height");
    _Size = glm::vec2(atof(width.c_str()), atof(height.c_str()));
}

int Level::GetPriority()
{
    return 0;
}

bool Level::OnMouseDown(pb::MouseButton button, glm::vec2 position)
{
    return false;
}

bool Level::OnMouseUp(pb::MouseButton button, glm::vec2 position)
{
    return false;
}

bool Level::OnMouseMove(glm::vec2 position)
{
    return false;
}

void Level::OnRecordRemoved(Project* project, Record* record)
{
    if (record == _Record)
    {
        Clear();
    }
}

void Level::OnEntityAdded(Record* record, Entity* entity)
{
    CreateEntity(entity);
}

void Level::OnEntityRemoved(Record* record, Entity* entity)
{
    DestroyEntity(entity->GetUid());
}

void Level::OnPropertyChanged(Struct* structure)
{
    UpdateSize();
}