#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/model.h"
#include "pixelboost/misc/stringHelpers.h"

#include "project/entity.h"
#include "project/schema.h"
#include "view/entity/property/model.h"
#include "view/entity/entity.h"
#include "view.h"

using namespace pixeleditor;

ModelViewProperty::ModelViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* schemaItem)
    : ViewProperty(parent, path, schemaItem)
{
    _ModelComponent = new pb::ModelComponent(parent, 0, 0);
    _ModelComponent->SetLayer(1);
    
    Refresh();
}

ModelViewProperty::~ModelViewProperty()
{
    _Parent->DestroyComponent(_ModelComponent);
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
    
    std::string model = EvaluateProperty(visualisation->GetParamValue("model"));
    
    if (model != _Model)
    {
        _Model = model;
        _ModelComponent->SetModel(View::Instance()->LoadModel(_Model));
        DirtyBounds();
    }
    
    std::string texture = EvaluateProperty(visualisation->GetParamValue("texture"));
    
    if (texture != _Texture)
    {
        _Texture = texture;
        _ModelComponent->SetTexture(View::Instance()->LoadTexture(_Texture));
    }
    
    if (visualisation->HasParamValue("rotation"))
    {
        std::vector<std::string> elements;
        std::string rotation = EvaluateProperty(visualisation->GetParamValue("rotation"));
        pb::StringHelpers::SplitString(rotation, ',', elements);
        
        if (elements.size() == 3)
        {
            glm::mat4x4 transform;
            transform = glm::rotate(transform, (float)atof(elements[0].c_str()), glm::vec3(1,0,0));
            transform = glm::rotate(transform, (float)atof(elements[1].c_str()), glm::vec3(0,1,0));
            transform = glm::rotate(transform, (float)atof(elements[2].c_str()), glm::vec3(0,0,1));
            _ModelComponent->SetLocalTransform(transform);
        }
    }
}

pb::BoundingBox ModelViewProperty::CalculateBounds()
{
    glm::vec3 size(1,1,1);
    
    glm::vec3 center = glm::vec3(_Parent->GetPosition()[0], _Parent->GetPosition()[1], 0);
    return pb::BoundingBox(center-size/2.f, center+size/2.f);
}
