#include "pixelboost/logic/component/transform/basic.h"

#include "core/uidHelpers.h"
#include "project/entity.h"
#include "view/entity/property/property.h"
#include "view/entity/entity.h"

using namespace pixeleditor;

PB_DEFINE_ENTITY(ViewProperty)

ViewProperty::ViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : pb::Entity(parent->GetScene(), parent, 0)
    , _Path(path)
    , _SchemaItem(schemaItem)
    , _BoundsDirty(true)
{
    CreateComponent<pb::BasicTransformComponent>();
    
    _PropertyId = GetViewEntity()->AddProperty(this);
}

ViewProperty::~ViewProperty()
{
    GetViewEntity()->RemoveProperty(this);
}

void ViewProperty::Update(float time)
{
    
}

void ViewProperty::Render(int layer)
{
    
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
