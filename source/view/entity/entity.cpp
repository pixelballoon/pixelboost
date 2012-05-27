#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "core/selection.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/property.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

ViewEntity::ViewEntity(Uid uid, Entity* entity)
    : _Uid(uid)
    , _Entity(entity)
{
    ParseProperties();
    
    _BoundsDirty = true;
    
    ResetTransform();
    
    _Entity->propertyChanged.Connect(this, &ViewEntity::OnPropertyChanged);
}

ViewEntity::~ViewEntity()
{
    _Entity->propertyChanged.Disconnect(this, &ViewEntity::OnPropertyChanged);
}

void ViewEntity::Update(float time)
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Update(time);
    }
}

void ViewEntity::Render(pb::RenderLayer* layer)
{
    UpdateBounds();
    
    View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(_Position.x, _Position.y), Vec2(0.1,0.1));
    View::Instance()->GetPrimitiveRenderer()->AttachLine(layer, Vec2(_Position.x, _Position.y), Vec2(_Position.x, _Position.y+1));
    View::Instance()->GetPrimitiveRenderer()->AttachLine(layer, Vec2(_Position.x, _Position.y), Vec2(_Position.x+1, _Position.y));
    
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Render(layer);
    }
    
    if (Core::Instance()->GetSelection().IsSelected(GenerateSelectionUid(GetUid())))
    {
        glm::vec3 boxCenter = _BoundingBox.GetCenter();
        glm::vec3 boxSize = _BoundingBox.GetSize();
        View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(boxCenter.x, boxCenter.y), Vec2(boxSize.x, boxSize.y), Vec3(0,0,0), Vec4(0.2,0.2,0.4,0.3));
           View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(boxCenter.x, boxCenter.y), Vec2(boxSize.x, boxSize.y), Vec3(0,0,0), Vec4(0.4,0.2,0.2,0.5), false);
    }
}

Uid ViewEntity::GetUid()
{
    return _Uid;
}

Entity* ViewEntity::GetEntity()
{
    return _Entity;
}

glm::vec3 ViewEntity::GetPosition()
{
    return _Position;
}

glm::vec3 ViewEntity::GetRotation()
{
    return _Rotation;
}

glm::vec3 ViewEntity::GetScale()
{
    return _Scale;
}

void ViewEntity::SetPosition(glm::vec3 position)
{
    _Position = position;
    DirtyBounds();
}

void ViewEntity::SetRotation(glm::vec3 rotation)
{
    _Rotation = rotation;
    DirtyBounds();
}

void ViewEntity::SetScale(glm::vec3 scale)
{
    _Scale = scale;
    DirtyBounds();
}

void ViewEntity::Transform(glm::vec3 positionOffset, glm::vec3 rotationOffset, glm::vec3 scaleOffset)
{
    _Position += positionOffset;
    _Rotation += rotationOffset;
    _Scale *= scaleOffset;
    DirtyBounds();
}

void ViewEntity::CommitTransform()
{
    _Entity->SetPosition(Vec3(_Position.x, _Position.y, _Position.z));
    _Entity->SetRotation(_Rotation.z);
    _Entity->SetScale(Vec3(_Scale.x, _Scale.y, _Scale.z));
}

void ViewEntity::ResetTransform()
{
    _Position = glm::vec3(_Entity->GetPosition()[0], _Entity->GetPosition()[1], _Entity->GetPosition()[2]);
    _Rotation = glm::vec3(0, 0, _Entity->GetRotation());
    _Scale = glm::vec3(_Entity->GetScale()[0], _Entity->GetScale()[1], _Entity->GetScale()[2]);
    DirtyBounds();
}

pb::BoundingBox ViewEntity::GetBoundingBox()
{
    if (_BoundsDirty)
        UpdateBounds();
    
    return _BoundingBox;
}

Uid ViewEntity::GetPropertyIdByPath(const std::string& path)
{
    PropertyIdMap::iterator it = _PropertyIdMap.find(path);
    
    if (it != _PropertyIdMap.end())
    {
        return it->second;
    }
    
    return 0;
}

ViewProperty* ViewEntity::GetPropertyByPath(const std::string& path)
{
    PropertyIdMap::iterator it = _PropertyIdMap.find(path);
    
    if (it != _PropertyIdMap.end())
    {
        return GetPropertyById(it->second);
    }
    
    return 0;
}

ViewProperty* ViewEntity::GetPropertyById(Uid uid)
{
    PropertyMap::iterator it = _Properties.find(uid);
    if (it != _Properties.end())
    {
        return it->second;
    }
    
    return 0;
}

void ViewEntity::RemoveProperty(ViewProperty* property)
{
    RemovePropertyById(property->GetPropertyId());
}

void ViewEntity::RemovePropertyById(Uid uid)
{
    PropertyMap::iterator it = _Properties.find(uid);
    if (it != _Properties.end())
    {
        delete it->second;
        _Properties.erase(it);
    }
}

Uid ViewEntity::GeneratePropertyId(const std::string& path)
{
    Uid uid;
    
    do {
        uid = (rand()%((1<<12)-2))+1;
    } while (_Properties.find(uid) != _Properties.end());
    
    _PropertyIdMap[path] = uid;
    
    return uid;
}

Uid ViewEntity::AddProperty(ViewProperty* property)
{
    ViewProperty* other = GetPropertyByPath(property->GetPath());
    
    if (other)
    {
        RemoveProperty(other);
    }
    
    Uid uid = GeneratePropertyId(property->GetPath());
    
    _Properties[uid] = property;
    
    return uid;
}

void ViewEntity::ParseProperties()
{
    const SchemaStruct* schemaStruct = _Entity->GetType();
    
    ParseItem("/", schemaStruct);
    ParseStruct("/", schemaStruct);
}

void ViewEntity::ParseStruct(const std::string& path, const SchemaStruct* schemaStruct)
{
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->second->GetName() + "/";
        
        ParseItem(propertyPath, it->second);
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                break;
            }
            case SchemaProperty::kSchemaPropertyArray:
            {
                break;
            }
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* propertyStruct = static_cast<SchemaPropertyStruct*>(it->second);
                ParseStruct(propertyPath, propertyStruct->GetSchemaStruct());
                break;
            }
            case SchemaProperty::kSchemaPropertyPointer:
            {
                break;
            }
        }
    }
}

void ViewEntity::ParseItem(const std::string& path, const SchemaItem* item)
{
    if (const SchemaAttribute* visualisation = item->GetAttribute("Visualisation"))
    {
        std::string type = visualisation->GetParamValue("type");
        
        if (type == "sprite")
        {
            new SpriteViewProperty(this, path, item);
        }
    }
}

void ViewEntity::OnPropertyChanged(pixeleditor::Struct* structure)
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Refresh();
    }
}

void ViewEntity::DirtyBounds()
{
    _BoundsDirty = true;
    
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->DirtyBounds();
    }
}

void ViewEntity::UpdateBounds()
{
    if (_BoundsDirty)
    {
        _BoundingBox.Invalidate();
        
        _BoundingBox.Expand(_Position);
        _BoundingBox.Expand(_Position + glm::vec3(1,1,0));
        
        for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
        {
            _BoundingBox.Expand(it->second->GetBoundingBox());
        }
        
        _BoundsDirty = false;
    }
}
