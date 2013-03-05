#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/component/transform/basic.h"
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

Level::Level(pb::Scene* scene)
    : Entity(scene, 0, 0)
    , _Record(0)
{
    View::Instance()->GetMouseManager()->AddHandler(this);
    
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &Level::OnRecordRemoved);
    
    CreateComponent<pb::BasicTransformComponent>();
    
    _LevelBounds = CreateComponent<pb::RectangleComponent>();
    _LevelBounds->SetColor(glm::vec4(0.6, 0.6, 0.6, 1.0));
    _LevelBounds->SetSolid(true);
    _LevelBounds->SetLayer(0);
    _LevelBounds->SetSize(glm::vec2(0,0));
}

Level::~Level()
{
    Core::Instance()->GetProject()->recordRemoved.Disconnect(this, &Level::OnRecordRemoved);
    
    View::Instance()->GetMouseManager()->RemoveHandler(this);
    
    Clear();
}

pb::Uid Level::GetType() const
{
    return Level::GetStaticType();
}

pb::Uid Level::GetStaticType()
{
    return pb::TypeHash("Level");
}

void Level::Update(float time)
{
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        it->second->Update(time);
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

void Level::SetRecord(ProjectRecord* record)
{
    Clear();
    
    if (record)
    {
        _Record = record;
        
        UpdateSize();
        
        _Record->propertyChanged.Connect(this, &Level::OnPropertyChanged);
        _Record->entityAdded.Connect(this, &Level::OnEntityAdded);
        _Record->entityRemoved.Connect(this, &Level::OnEntityRemoved);
        
        for (ProjectRecord::EntityMap::const_iterator it = record->GetEntities().begin(); it != record->GetEntities().end(); ++it)
        {
            ProjectEntity* entity = it->second;
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
    ProjectEntity* entity = _Record->GetEntity(uid);

    if (entity)
        CreateEntity(entity);
}

void Level::CreateEntity(ProjectEntity* entity)
{
    ViewEntity* viewEntity = new ViewEntity(GetScene(), entity);
    _Entities[viewEntity->GetEntityUid()] = viewEntity;
    entityAdded(viewEntity);
}

void Level::DestroyEntity(ProjectEntity* entity)
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
    EntityMap::iterator it = _Entities.find(entity->GetEntityUid());
    
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
    
    if (!_Record || !_Record->GetType())
        return;

    const SchemaAttribute* hasLevel = _Record->GetType()->GetAttribute("HasLevel");
    if (!hasLevel)
        return;
    
    float width = hasLevel->EvaluateParamFloat(_Record, "width", "/", 0.f);
    float height = hasLevel->EvaluateParamFloat(_Record, "height", "/", 0.f);
    std::string origin = hasLevel->EvaluateParamString(_Record, "origin", "/");
    
    glm::vec2 size = glm::vec2(width, height);
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

int Level::GetInputHandlerPriority()
{
    return 1;
}

bool Level::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifier, glm::vec2 position)
{
    return false;
}

bool Level::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifier, glm::vec2 position)
{
    return false;
}

bool Level::OnMouseMove(glm::vec2 position)
{
    return false;
}

void Level::OnRecordRemoved(Project* project, ProjectRecord* record)
{
    if (record == _Record)
    {
        Clear();
    }
}

void Level::OnEntityAdded(ProjectRecord* record, ProjectEntity* entity)
{
    CreateEntity(entity);
}

void Level::OnEntityRemoved(ProjectRecord* record, ProjectEntity* entity)
{
    DestroyEntity(entity);
}

void Level::OnPropertyChanged(ProjectStruct* structure, Property* property)
{
    UpdateSize();
}
