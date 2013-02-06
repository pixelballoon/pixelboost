#include "core/uidHelpers.h"
#include "project/entity.h"
#include "view/entity/property/property.h"
#include "view/entity/entity.h"

using namespace pixeleditor;

ViewProperty::ViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : _Parent(parent)
    , _Path(path)
    , _SchemaItem(schemaItem)
    , _BoundsDirty(true)
{
    _PropertyId = _Parent->AddProperty(this);
}

ViewProperty::~ViewProperty()
{
    _Parent->RemoveProperty(this);
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

ProjectEntity* ViewProperty::GetProjectEntity()
{
    return _Parent->GetEntity();
}

Uid ViewProperty::GetUid()
{
    return GenerateSelectionUid(_Parent->GetUid(), _PropertyId);
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
        _Parent->DirtyBounds();
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
