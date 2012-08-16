#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/component/transform/basic.h"

#include "core/selection.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/model.h"
#include "view/entity/property/property.h"
#include "view/entity/property/sprite.h"
#include "view/entity/entity.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

ViewEntity::ViewEntity(pb::Scene* scene, pixeleditor::Entity* entity)
    : pb::Entity(scene, 0)
    , _BoundsComponent(0)
    , _Entity(entity)
{
    new pb::BasicTransformComponent(this);

    ResetTransform();
    
    ParseProperties();
    
    _Entity->propertyChanged.Connect(this, &ViewEntity::OnPropertyChanged);
    _Entity->destroyed.Connect(this, &ViewEntity::OnDestroyed);
    
    Core::Instance()->GetSelection().selectionChanged.Connect(this, &ViewEntity::OnSelectionChanged);
    
    OnSelectionChanged(&Core::Instance()->GetSelection());
}

ViewEntity::~ViewEntity()
{
    Core::Instance()->GetSelection().selectionChanged.Disconnect(this, &ViewEntity::OnSelectionChanged);
    
    if (_Entity)
    {
        _Entity->propertyChanged.Disconnect(this, &ViewEntity::OnPropertyChanged);
        _Entity->destroyed.Disconnect(this, &ViewEntity::OnDestroyed);
    }
}

pb::Uid ViewEntity::GetType() const
{
    return ViewEntity::GetStaticType();
}

pb::Uid ViewEntity::GetStaticType()
{
    return pb::TypeHash("ViewEntity");
}

void ViewEntity::Update(float time)
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        it->second->Update(time);
    }
}

void ViewEntity::Render(int layer)
{

}

Entity* ViewEntity::GetEntity()
{
    return _Entity;
}

pb::Uid ViewEntity::GetEntityUid()
{
    if (_Entity)
        return _Entity->GetUid();
    return 0;
}

glm::vec3 ViewEntity::GetPosition()
{
    return GetComponentByType<pb::TransformComponent>()->GetPosition();
}

glm::vec3 ViewEntity::GetRotation()
{
    return GetComponentByType<pb::TransformComponent>()->GetRotation();
}

glm::vec3 ViewEntity::GetScale()
{
    return GetComponentByType<pb::TransformComponent>()->GetScale();
}

void ViewEntity::SetPosition(glm::vec3 position)
{
    GetComponentByType<pb::TransformComponent>()->SetPosition(position);
    DirtyBounds();
}

void ViewEntity::SetRotation(glm::vec3 rotation)
{
    GetComponentByType<pb::TransformComponent>()->SetRotation(rotation);
    DirtyBounds();
}

void ViewEntity::SetScale(glm::vec3 scale)
{
    GetComponentByType<pb::TransformComponent>()->SetScale(scale);
    DirtyBounds();
}

void ViewEntity::Transform(glm::vec3 positionOffset, glm::vec3 rotationOffset, glm::vec3 scaleOffset)
{
    GetComponentByType<pb::TransformComponent>()->SetTransform(GetPosition()+positionOffset, GetRotation()+rotationOffset, GetScale()*scaleOffset);
    DirtyBounds();
}

void ViewEntity::CommitTransform()
{
    _Entity->SetPosition(GetPosition());
    _Entity->SetRotation(GetRotation().z);
    _Entity->SetScale(GetScale());
}

void ViewEntity::ResetTransform()
{
    GetComponentByType<pb::TransformComponent>()->SetTransform(_Entity->GetPosition(), glm::vec3(0,0,_Entity->GetRotation()), _Entity->GetScale());
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

void ViewEntity::OnSelectionChanged(const pixeleditor::Selection* selection)
{
    if (selection->IsSelected(GenerateSelectionUid(GetEntityUid())))
    {
        UpdateBounds();
        
        if (!_BoundsComponent)
        {
            _BoundsComponent = new pb::RectangleComponent(this);
            _BoundsComponent->SetColor(glm::vec4(0.2, 0.2, 0.4, 0.3));
            _BoundsComponent->SetSolid(true);
            _BoundsComponent->SetLayer(2);
            _BoundsComponent->SetSize(glm::vec2(_BoundingBox.GetSize().x, _BoundingBox.GetSize().y)/glm::vec2(GetScale().x, GetScale().y));
        }
    } else {
        if (_BoundsComponent)
        {
            DestroyComponent(_BoundsComponent);
            _BoundsComponent = 0;
        }
    }
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
        } else if (type == "model")
        {
            new ModelViewProperty(this, path, item);
        }
    }
}

void ViewEntity::OnDestroyed(pixeleditor::Struct* structure)
{
    _Entity->propertyChanged.Disconnect(this, &ViewEntity::OnPropertyChanged);
    _Entity->destroyed.Disconnect(this, &ViewEntity::OnDestroyed);
    _Entity = 0;
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
        
        _BoundingBox.Expand(GetPosition());
        _BoundingBox.Expand(GetPosition() + glm::vec3(1,1,0));
        
        for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
        {
            _BoundingBox.Expand(it->second->GetBoundingBox());
        }
        
        _BoundsDirty = false;
    }
}
