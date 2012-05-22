#include "project/entity.h"
#include "view/entity/property/property.h"
#include "view/entity/entity.h"

using namespace pixeleditor;

ViewProperty::ViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : _Parent(parent)
    , _Path(path)
    , _SchemaItem(schemaItem)
{
    
}

ViewProperty::~ViewProperty()
{
    
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

std::string ViewProperty::EvaluateProperty(const std::string& path, const std::string& defaultValue)
{
    if (path.length() == 0)
        return defaultValue;
    
    if (path[0] != '/')
        return path;
    
    return _Parent->GetEntity()->EvaluateProperty(_Path.substr(0, _Path.length()-1) + path, defaultValue);
}
