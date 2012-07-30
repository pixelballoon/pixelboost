#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/components/rectangle.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/scene.h"

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

Level::Level(pb::Scene* scene)
    : Entity(scene, 0)
    , _Record(0)
{
    View::Instance()->GetMouseManager()->AddHandler(this);
    
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &Level::OnRecordRemoved);
    
    _LevelBounds = new pb::RectangleComponent(this);
    _LevelBounds->SetColor(glm::vec4(0.6, 0.6, 0.6, 1.0));
    _LevelBounds->SetSolid(true);
    _LevelBounds->SetLayer(0);
    _LevelBounds->SetSize(glm::vec2(0,0));
        
    AddComponent(_LevelBounds);
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

void Level::Render(int backgroundLayer, int levelLayer)
{
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
        DestroyEntity(_Entities.begin()->second);
    
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
            pixeleditor::Entity* entity = it->second;
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

Level::EntityList Level::GetEntitiesInBounds(const pb::BoundingBox &bounds)
{
    EntityList entities;
    
    for (Level::EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetBoundingBox().Intersects(bounds))
        {
            entities.push_back(it->second);
        }
    }
    
    return entities;
}
                         
void Level::CreateEntity(Uid uid)
{
    pixeleditor::Entity* entity = _Record->GetEntity(uid);

    if (entity)
        CreateEntity(entity);
}

void Level::CreateEntity(pixeleditor::Entity* entity)
{
    ViewEntity* viewEntity = new ViewEntity(GetScene(), entity);
    _Entities[viewEntity->GetUid()] = viewEntity;
    entityAdded(viewEntity);
}

void Level::DestroyEntity(pixeleditor::Entity* entity)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetEntity() == entity)
        {
            DestroyEntity(it->second);
            return;
        }
    }
}

void Level::DestroyEntity(ViewEntity* entity)
{
    EntityMap::iterator it = _Entities.find(entity->GetUid());
    
    if (it != _Entities.end())
    {
        GetScene()->DestroyEntity(it->second);
        entityRemoved(it->second);
        _Entities.erase(it);
    }
}

void Level::UpdateSize()
{
    _LevelBounds->SetSize(glm::vec2(0,0));
    
    if (!_Record)
        return;

    const SchemaAttribute* hasLevel = _Record->GetType()->GetAttribute("HasLevel");
    if (!hasLevel)
        return;
    
    std::string width = hasLevel->EvaluateParamValue(_Record, "width");
    std::string height = hasLevel->EvaluateParamValue(_Record, "height");
    std::string origin = hasLevel->EvaluateParamValue(_Record, "origin");
    
    glm::vec2 size = glm::vec2(atof(width.c_str()), atof(height.c_str()));
    glm::vec3 offset;
    
    if (origin.length() == 2)
    {
        switch (origin.at(0))
        {
            case 'b':
                offset.y = size.y/2.f;
                break;
            case 't':
                offset.y = -size.y/2.f;
                break;
        }
        switch (origin.at(1))
        {
            case 'l':
                offset.x = size.x/2.f;
                break;
            case 'r':
                offset.x = -size.x/2.f;
                break;
        }
    }
    
    _LevelBounds->SetSize(size);
    _LevelBounds->SetLocalTransform(glm::translate(glm::mat4x4(), offset));
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

void Level::OnEntityAdded(Record* record, pixeleditor::Entity* entity)
{
    CreateEntity(entity);
}

void Level::OnEntityRemoved(Record* record, pixeleditor::Entity* entity)
{
    DestroyEntity(entity);
}

void Level::OnPropertyChanged(Struct* structure)
{
    UpdateSize();
}