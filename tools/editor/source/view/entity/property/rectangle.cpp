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

PB_DEFINE_ENTITY(RectangleViewProperty)

RectangleViewProperty::RectangleViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : ViewProperty(scene, parent, creationEntity)
{
    
}

RectangleViewProperty::~RectangleViewProperty()
{
    
}

void RectangleViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    CreateComponent<pb::RectangleComponent>()->SetLayer(1);
    
    Refresh();
}

void RectangleViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    glm::vec2 size = visualisation->EvaluateParamVector2(GetProjectEntity(), "size", GetPath(), glm::vec2(1,1));
    bool solid = visualisation->EvaluateParamBool(GetProjectEntity(), "solid", GetPath(), true);
    glm::vec4 color = visualisation->EvaluateParamColor(GetProjectEntity(), "color", GetPath());
    
    pb::RectangleComponent* rectangle = GetComponent<pb::RectangleComponent>();
    rectangle->SetSize(size);
    rectangle->SetSolid(solid);
    rectangle->SetColor(color);
}

pb::BoundingBox RectangleViewProperty::CalculateBounds()
{
    glm::vec3 size(GetComponent<pb::RectangleComponent>()->GetSize(), 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
