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

ModelComponent::ModelComponent(Entity* parent, Model* model, Texture* texture)
    : Component(parent)
{
    _Renderable = new ModelRenderable(parent->GetUid());
    _Renderable->SetModel(model);
    _Renderable->SetTexture(texture);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    UpdateTransform();
}

ModelComponent::~ModelComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid ModelComponent::GetType()
{
    return GetStaticType();
}

Uid ModelComponent::GetStaticType()
{
    return TypeHash("pb::ModelComponent");
}

void ModelComponent::SetShader(Shader* shader)
{
    _Renderable->SetShader(shader);
}

void ModelComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void ModelComponent::SetModel(Model* model)
{
    _Renderable->SetModel(model);
}

Model* ModelComponent::GetModel()
{
    return _Renderable->GetModel();
}

void ModelComponent::SetTexture(Texture* texture)
{
    _Renderable->SetTexture(texture);
}

Texture* ModelComponent::GetTexture()
{
    return _Renderable->GetTexture();
}

void ModelComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->SetTint(tint);
}

void ModelComponent::SetAlphaBlend(bool alphaBlend)
{
    _Renderable->SetAlphaBlend(alphaBlend);
}

void ModelComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void ModelComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void ModelComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}

const ModelRenderable* ModelComponent::GetRenderable() const
{
    return _Renderable;
}