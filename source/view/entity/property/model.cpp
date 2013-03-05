#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/model.h"
#include "pixelboost/misc/stringHelpers.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/model.h"
#include "view/entity/entity.h"
#include "view.h"

ModelViewProperty::ModelViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : ViewProperty(parent, path, schemaItem)
{
    CreateComponent<pb::ModelComponent>()->SetLayer(1);
    
    Refresh();
}

ModelViewProperty::~ModelViewProperty()
{
    DestroyComponent(GetComponent<pb::ModelComponent>());
}

void ModelViewProperty::Update(float time)
{
    ViewProperty::Update(time);
}

void ModelViewProperty::Render(int layer)
{
    ViewProperty::Render(layer);
}

void ModelViewProperty::Refresh()
{
    const SchemaAttribute* visualisation = _SchemaItem->GetAttribute("Visualisation");
    
    std::string model = visualisation->EvaluateParamString(GetProjectEntity(), "model", GetPath());
    
    if (model != _Model)
    {
        _Model = model;
        GetComponent<pb::ModelComponent>()->SetModel(View::Instance()->GetModelRenderer()->LoadModel(_Model, "editor_models/"+_Model));
        DirtyBounds();
    }
    
    std::string texture = visualisation->EvaluateParamString(GetProjectEntity(), "texture", GetPath());
    
    if (texture != _Texture)
    {
        _Texture = texture;
        GetComponent<pb::ModelComponent>()->SetTexture(View::Instance()->GetModelRenderer()->LoadTexture(_Texture, "editor_images/"+_Texture));
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
