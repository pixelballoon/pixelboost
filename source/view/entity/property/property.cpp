#include "pixelboost/logic/component/transform.h"

#include "core/uidHelpers.h"
#include "project/entity.h"
#include "view/entity/property/property.h"
#include "view/entity/entity.h"

PB_DEFINE_ENTITY(ViewProperty)

ViewProperty::ViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : pb::Entity(scene, parent, creationEntity)
    , _BoundsDirty(true)
{
    CreateComponent<pb::TransformComponent>();
    
    _PropertyId = GetViewEntity()->AddProperty(this);
}

ViewProperty::~ViewProperty()
{
    if (GetViewEntity())
        GetViewEntity()->RemoveProperty(this);
}

void ViewProperty::Initialise(const std::string& path, const SchemaItem* schemaItem)
{
    _Path = path;
    _SchemaItem = schemaItem;
}

void ViewProperty::Refresh()
{
    
}

ViewEntity* ViewProperty::GetViewEntity()
{
    return static_cast<ViewEntity*>(GetParent());
}

ProjectEntity* ViewProperty::GetProjectEntity()
{
    return GetViewEntity()->GetEntity();
}

Uid ViewProperty::GetUid()
{
    return GenerateSelectionUid(GetParent()->GetUid(), _PropertyId);
}


Uid ViewProperty::GetPropertyId()
{
    return _PropertyId;
}

const std::string& ViewProperty::GetPath()
{
    return _Path;
}

void ViewProperty::DirtyBounds()
{
    if (!_BoundsDirty)
    {
        _BoundsDirty = true;
        GetViewEntity()->DirtyBounds();
    }
}

pb::BoundingBox ViewProperty::GetBoundingBox()
{
    if (_BoundsDirty)
    {
        _BoundingBox = CalculateBounds();
        _BoundsDirty = false;
    }
    
    return _BoundingBox;
}

pb::BoundingBox ViewProperty::CalculateBounds()
{
    return pb::BoundingBox();
}
