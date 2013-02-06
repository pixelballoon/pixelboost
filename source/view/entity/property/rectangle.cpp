#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"

#include "core.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/schema.h"
#include "view/entity/property/rectangle.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

RectangleViewProperty::RectangleViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : ViewProperty(parent, path, schemaItem)
{
    _RectangleComponent = new pb::RectangleComponent(parent);
    _RectangleComponent->SetLayer(1);
    
    Refresh();
}

RectangleViewProperty::~RectangleViewProperty()
{
    _Parent->DestroyComponent(_RectangleComponent);
}

void RectangleViewProperty::Update(float time)
{
    ViewProperty::Update(time);
}

void RectangleViewProperty::Render(int layer)
{
    ViewProperty::Render(layer);
}

void RectangleViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    float height = visualisation->EvaluateParamFloat(GetProjectEntity(), "height", GetPath(), 1.f);
    float width = visualisation->EvaluateParamFloat(GetProjectEntity(), "width", GetPath(), 1.f);
    
    _RectangleComponent->SetSize(glm::vec2(width, height));
    
    bool solid = visualisation->EvaluateParamBool(GetProjectEntity(), "solid", GetPath(), true);
    
    _RectangleComponent->SetSolid(solid);
}

pb::BoundingBox RectangleViewProperty::CalculateBounds()
{
    glm::vec3 size(_RectangleComponent->GetSize(), 0);
    size *= _Parent->GetScale();
    
    glm::vec3 center = glm::vec3(_Parent->GetPosition()[0], _Parent->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
