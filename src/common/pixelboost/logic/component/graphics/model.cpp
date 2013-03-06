#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/asset/model.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
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

}

void ModelComponent::SetShader(Shader* shader)
{
    GetRenderable()->SetShader(shader);
}

void ModelComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void ModelComponent::SetModel(Model* model)
{
    GetRenderable()->SetModel(model);
}

Model* ModelComponent::GetModel()
{
    return GetRenderable()->GetModel();
}

void ModelComponent::SetTexture(Texture* texture)
{
    GetRenderable()->SetTexture(texture);
}

Texture* ModelComponent::GetTexture()
{
    return GetRenderable()->GetTexture();
}

void ModelComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
}

void ModelComponent::SetAlphaBlend(bool alphaBlend)
{
    GetRenderable()->SetAlphaBlend(alphaBlend);
}
