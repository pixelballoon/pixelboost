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

void ViewProperty::Render(pb::RenderLayer* layer)
{
    
}

void ViewProperty::Refresh()
{
    
}

Uid ViewProperty::GetUid()
{
    return _Parent->GetUid() | _PropertyId << 12;
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
    _BoundsDirty = true;
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

std::string ViewProperty::EvaluateProperty(const std::string& path, const std::string& defaultValue)
{
    if (path.length() == 0)
        return defaultValue;
    
    if (path[0] != '/')
        return path;
    
    return _Parent->GetEntity()->EvaluateProperty(_Path.substr(0, _Path.length()-1) + path, defaultValue);
}
