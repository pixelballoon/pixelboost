#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/logic/component/graphics/particleEmitter.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/particle/finished.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

ParticleEmitterComponent::ParticleEmitterComponent(Entity* parent, int maxParticles)
    : Component(parent)
{
    _Renderable = new ParticleRenderable(parent->GetUid(), maxParticles);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &ParticleEmitterComponent::OnUpdate));
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &ParticleEmitterComponent::OnTransformChanged));
    
    UpdateTransform();
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
    GetParent()->UnregisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &ParticleEmitterComponent::OnUpdate));
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &ParticleEmitterComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid ParticleEmitterComponent::GetType()
{
    return GetStaticType();
}

Uid ParticleEmitterComponent::GetStaticType()
{
    return TypeHash("pb::ParticleEmitterComponent");
}

void ParticleEmitterComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void ParticleEmitterComponent::SetLocalTransform(const glm::mat4x4& localTransform)
{
    _LocalTransform = localTransform;
}

ParticleEmitter* ParticleEmitterComponent::GetEmitter()
{
    return _Renderable->GetEmitter();
}

void ParticleEmitterComponent::OnUpdate(const Message& message)
{
    bool wasFinished = _Renderable->GetEmitter()->IsFinished();
    
    _Renderable->GetEmitter()->Update(static_cast<const UpdateMessage&>(message).GetDelta());
    
    if (!wasFinished && _Renderable->GetEmitter()->IsFinished())
    {
        ParticleFinishedMessage message(GetParent(), this);
        GetScene()->SendMessage(GetParentUid(), message);
    }
}

void ParticleEmitterComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void ParticleEmitterComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        glm::mat4x4 matrix = transform->GetMatrix() * _LocalTransform;
        _Renderable->GetEmitter()->SetPosition(glm::vec3(matrix[3].x, matrix[3].y, matrix[3].z));
    }
}
