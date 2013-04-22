#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/ellipse.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/ellipse.h"
#include "view/entity/entity.h"

PB_DEFINE_ENTITY(EllipseViewProperty)

EllipseViewProperty::EllipseViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : ViewProperty(scene, parent, creationEntity)
{
    
}

EllipseViewProperty::~EllipseViewProperty()
{
    
}

void EllipseViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    CreateComponent<pb::EllipseComponent>()->SetLayer(1);
    
    Refresh();
}

void EllipseViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    glm::vec2 size = visualisation->EvaluateParamVector2(GetProjectEntity(), "size", GetPath(), glm::vec2(1,1));
    bool solid = visualisation->EvaluateParamBool(GetProjectEntity(), "solid", GetPath(), true);
    glm::vec4 color = visualisation->EvaluateParamColor(GetProjectEntity(), "color", GetPath());
    
    pb::EllipseComponent* rectangle = GetComponent<pb::EllipseComponent>();
    rectangle->SetSize(size);
    rectangle->SetSolid(solid);
    rectangle->SetColor(color);
}

pb::BoundingBox EllipseViewProperty::CalculateBounds()
{
    glm::vec3 size(GetComponent<pb::EllipseComponent>()->GetSize(), 0);
    size *= GetViewEntity()->GetScale();
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
