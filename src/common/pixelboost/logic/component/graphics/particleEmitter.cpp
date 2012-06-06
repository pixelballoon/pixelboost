#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/logic/component/graphics/particleEmitter.h"
#include "pixelboost/logic/component/transform.h"
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
    
    GetParent()->RegisterMessageHandler(UpdateMessage::GetStaticType(), Entity::MessageHandler(this, &ParticleEmitterComponent::OnUpdate));
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &ParticleEmitterComponent::OnTransformChanged));
    
    UpdateTransform();
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
    GetParent()->UnregisterMessageHandler(UpdateMessage::GetStaticType(), Entity::MessageHandler(this, &ParticleEmitterComponent::OnUpdate));
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &ParticleEmitterComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid ParticleEmitterComponent::GetType()
{
    return GetStaticType();
}

Uid ParticleEmitterComponent::GetStaticType()
{
    return TypeHash("particle");
}

void ParticleEmitterComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

ParticleEmitter* ParticleEmitterComponent::GetEmitter()
{
    return _Renderable->GetEmitter();
}

void ParticleEmitterComponent::OnUpdate(Uid sender, Message& message)
{
    _Renderable->GetEmitter()->Update(static_cast<UpdateMessage&>(message).GetDelta());
}

void ParticleEmitterComponent::OnTransformChanged(Uid sender, Message& message)
{
    UpdateTransform();
}

void ParticleEmitterComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->GetEmitter()->SetPosition(transform->GetPosition());
    }
}
