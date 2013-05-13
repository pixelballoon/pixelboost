#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/asset/model.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/resources/modelResource.h"
#include "pixelboost/logic/component/graphics/model.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

template class pb::RenderableComponent<pb::ModelRenderable>;

PB_DEFINE_COMPONENT(pb::ModelComponent)

ModelComponent::ModelComponent(Entity* parent)
    : RenderableComponent<pb::ModelRenderable>(parent)
{

}

ModelComponent::~ModelComponent()
{
    if (_Model)
    {
        _Model->SignalResourceLoaded.Disconnect(this, &ModelComponent::OnResourceLoaded);
        _Model->SignalResourceUnloading.Disconnect(this, &ModelComponent::OnResourceUnloading);
        _Model.reset();
    }
}

void ModelComponent::SetShader(Shader* shader)
{
    GetRenderable()->SetShader(shader);
}

void ModelComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void ModelComponent::SetModel(const std::string& filename)
{
    if (_Model)
    {
        _Model->SignalResourceLoaded.Disconnect(this, &ModelComponent::OnResourceLoaded);
        _Model->SignalResourceUnloading.Disconnect(this, &ModelComponent::OnResourceUnloading);
        _Model.reset();
    }
    
    _Model = pb::ResourceManager::Instance()->GetPool("default")->GetResource<pb::ModelResource>(filename);
    _Model->SignalResourceLoaded.Connect(this, &ModelComponent::OnResourceLoaded);
    _Model->SignalResourceUnloading.Connect(this, &ModelComponent::OnResourceUnloading);
    
    if (_Model->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Model.get(), false);
    }
}

Model* ModelComponent::GetModel()
{
    return GetRenderable()->GetModel();
}

void ModelComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
}

const glm::vec4& ModelComponent::GetTint()
{
    return GetRenderable()->GetTint();
}

void ModelComponent::OnResourceLoaded(Resource* resource, bool error)
{
    if (!error)
    {
        if (resource == _Model.get())
        {
            GetRenderable()->SetModel(new Model(&_Model->GetModelDefinition()));
        }
    }
}

void ModelComponent::OnResourceUnloading(Resource* resource)
{
    if (resource == _Model.get())
    {
        GetRenderable()->SetModel(0);
    }
}
