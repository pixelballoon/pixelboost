#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/scene.h"

#include "core/selection.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/ellipse.h"
#include "view/entity/property/model.h"
#include "view/entity/property/property.h"
#include "view/entity/property/rectangle.h"
#include "view/entity/property/sprite.h"
#include "view/entity/property/text.h"
#include "view/entity/entity.h"
#include "core.h"
#include "view.h"

PB_DEFINE_ENTITY(ViewEntity)

ViewEntity::ViewEntity(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : pb::Entity(scene, parent, creationEntity)
    , _Entity(0)
{
    
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

void ViewEntity::Initialise(ProjectEntity* entity)
{
    _Entity = entity;
    
    CreateComponent<pb::TransformComponent>();
    
    ResetTransform();
    
    ParseProperties();
    
    _Entity->propertyChanged.Connect(this, &ViewEntity::OnPropertyChanged);
    _Entity->destroyed.Connect(this, &ViewEntity::OnDestroyed);
    
    Core::Instance()->GetSelection().selectionChanged.Connect(this, &ViewEntity::OnSelectionChanged);
    
    OnSelectionChanged(&Core::Instance()->GetSelection());
}

ProjectEntity* ViewEntity::GetEntity()
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
    return GetComponent<pb::TransformComponent>()->GetPosition();
}

glm::vec3 ViewEntity::GetRotation()
{
    return GetComponent<pb::TransformComponent>()->GetRotation();
}

glm::vec3 ViewEntity::GetScale()
{
    return GetComponent<pb::TransformComponent>()->GetScale();
}

void ViewEntity::SetPosition(glm::vec3 position)
{
    GetComponent<pb::TransformComponent>()->SetPosition(position);
    DirtyBounds();
}

void ViewEntity::SetRotation(glm::vec3 rotation)
{
    GetComponent<pb::TransformComponent>()->SetRotation(rotation);
    DirtyBounds();
}

void ViewEntity::SetScale(glm::vec3 scale)
{
    GetComponent<pb::TransformComponent>()->SetScale(scale);
    DirtyBounds();
}

void ViewEntity::Transform(glm::vec3 positionOffset, glm::vec3 rotationOffset, glm::vec3 scaleOffset)
{
    GetComponent<pb::TransformComponent>()->SetTransform(GetPosition()+positionOffset, GetRotation()+rotationOffset, GetScale()*scaleOffset);
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
    GetComponent<pb::TransformComponent>()->SetTransform(_Entity->GetPosition(), glm::vec3(0,0,_Entity->GetRotation()), _Entity->GetScale());
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
        it->second->Destroy();
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

void ViewEntity::OnUpdate(const pb::Message& message)
{
    pb::RectangleComponent* bounds = GetComponent<pb::RectangleComponent>();
    
    if (bounds)
    {
        UpdateBounds();
        bounds->SetSize(glm::vec2(_BoundingBox.GetSize().x, _BoundingBox.GetSize().y)/glm::vec2(GetScale().x, GetScale().y));
    }
    
    UnregisterMessageHandler<pb::UpdateMessage>(pb::MessageHandler(this, &ViewEntity::OnUpdate));
}

void ViewEntity::OnSelectionChanged(const Selection* selection)
{
    pb::RectangleComponent* bounds = GetComponent<pb::RectangleComponent>();
    
    if (selection->IsSelected(GenerateSelectionUid(GetEntityUid())))
    {
        UpdateBounds();
        
        if (!bounds)
        {
            bounds = CreateComponent<pb::RectangleComponent>();
            bounds->SetColor(glm::vec4(0.2, 0.2, 0.4, 0.3));
            bounds->SetSolid(true);
            bounds->SetLayer(2);
            bounds->SetSize(glm::vec2(_BoundingBox.GetSize().x, _BoundingBox.GetSize().y)/glm::vec2(GetScale().x, GetScale().y));
        }
    } else {
        if (bounds)
        {
            DestroyComponent(bounds);
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
    
	ParseStruct("/", schemaStruct);
}

void ViewEntity::ParseStruct(const std::string& path, const SchemaStruct* schemaStruct)
{
	if (!schemaStruct)
		return;
	
	ParseItem(path, schemaStruct);
	
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->second->GetName() + "/";
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
				ParseItem(propertyPath, it->second);
                break;
            }
            case SchemaProperty::kSchemaPropertyArray:
            {
				ParseItem(propertyPath, it->second);
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
				ParseItem(propertyPath, it->second);
                break;
            }
            case SchemaProperty::kSchemaPropertyReference:
            {
				ParseItem(propertyPath, it->second);
                break;
            }
        }
    }
	
	ParseStruct(path, schemaStruct->GetBaseType());
}

void ViewEntity::ParseItem(const std::string& path, const SchemaItem* item)
{
    if (const SchemaAttribute* visualisation = item->GetAttribute("Visualisation"))
    {
        std::string type = visualisation->GetParamValue("type");
        
        if (type == "sprite")
        {
            GetScene()->CreateEntity<SpriteViewProperty>(this, 0)->Initialise(path, item);
        } else if (type == "model")
        {
            GetScene()->CreateEntity<ModelViewProperty>(this, 0)->Initialise(path, item);
        } else if (type == "rectangle")
        {
            GetScene()->CreateEntity<RectangleViewProperty>(this, 0)->Initialise(path, item);
        } else if (type == "text")
        {
            GetScene()->CreateEntity<TextViewProperty>(this, 0)->Initialise(path, item);
        } else if (type == "ellipse")
        {
            GetScene()->CreateEntity<EllipseViewProperty>(this, 0)->Initialise(path, item);
        } else {
            PbLogDebug("view.entity.property", "Unknown property type %s", type.c_str());
        }
    }
}

void ViewEntity::OnDestroyed(ProjectStruct* structure)
{
    _Entity->propertyChanged.Disconnect(this, &ViewEntity::OnPropertyChanged);
    _Entity->destroyed.Disconnect(this, &ViewEntity::OnDestroyed);
    _Entity = 0;
}

void ViewEntity::OnPropertyChanged(ProjectStruct* structure, Property* property)
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
    
    if (GetComponent<pb::RectangleComponent>())
    {
        RegisterMessageHandler<pb::UpdateMessage>(pb::MessageHandler(this, &ViewEntity::OnUpdate));
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
