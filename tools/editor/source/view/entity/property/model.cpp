#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/data/stringHelpers.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/model.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/model.h"
#include "view/entity/entity.h"
#include "view.h"

PB_DEFINE_ENTITY(ModelViewProperty)

ModelViewProperty::ModelViewProperty(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : ViewProperty(scene, parent, creationEntity)
{
    
}

ModelViewProperty::~ModelViewProperty()
{
    
}

void ModelViewProperty::Initialise(const std::string &path, const SchemaItem *schemaItem)
{
    ViewProperty::Initialise(path, schemaItem);
    
    CreateComponent<pb::ModelComponent>()->SetLayer(1);
    
    Refresh();
}

void ModelViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string model = visualisation->EvaluateParamString(GetProjectEntity(), "model", GetPath());
    
    if (model != _Model)
    {
        _Model = model;
        GetComponent<pb::ModelComponent>()->SetModel(_Model);
        DirtyBounds();
    }
    
    std::string material = visualisation->EvaluateParamString(GetProjectEntity(), "material", GetPath());
    
    if (material != _Material)
    {
        _Material = material;
        GetComponent<pb::ModelComponent>()->SetMaterial(_Material);
    }
    
    if (visualisation->HasParamValue("rotation"))
    {
        glm::vec3 rotation = visualisation->EvaluateParamVector3(GetProjectEntity(), "rotation", GetPath(), glm::vec3(0,0,0));
        
        glm::mat4x4 transform;
        transform = glm::rotate(transform, rotation.x, glm::vec3(1,0,0));
        transform = glm::rotate(transform, rotation.y, glm::vec3(0,1,0));
        transform = glm::rotate(transform, rotation.z, glm::vec3(0,0,1));
        GetComponent<pb::ModelComponent>()->SetLocalTransform(transform);
    }
}

pb::BoundingBox ModelViewProperty::CalculateBounds()
{
    glm::vec3 size(1,1,1);
    
    glm::vec3 center = glm::vec3(GetViewEntity()->GetPosition()[0], GetViewEntity()->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
