#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/logic/component/graphics/model.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

ModelComponent::ModelComponent(Entity* parent, const std::string& model, const std::string& texture)
    : Component(parent)
{
    _Renderable = new ModelRenderable(parent->GetUid());
    _Renderable->SetModel(model);
    _Renderable->SetTexture(texture);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    UpdateTransform();
}

ModelComponent::~ModelComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &ModelComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid ModelComponent::GetType()
{
    return GetStaticType();
}

Uid ModelComponent::GetStaticType()
{
    return TypeHash("model");
}

void ModelComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void ModelComponent::SetModel(const std::string& model)
{
    _Renderable->SetModel(model);
}

void ModelComponent::SetTexture(const std::string& texture)
{
    _Renderable->SetTexture(texture);
}

void ModelComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->SetTint(tint);
}

void ModelComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void ModelComponent::OnTransformChanged(Uid sender, const Message& message)
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