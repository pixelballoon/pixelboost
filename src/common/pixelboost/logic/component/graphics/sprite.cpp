#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/graphics/sprite.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::SpriteComponent)

SpriteComponent::SpriteComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new SpriteRenderable(parent->GetUid());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetEntity()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SpriteComponent::OnTransformChanged));
    
    UpdateTransform();
}

SpriteComponent::~SpriteComponent()
{
    GetEntity()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &SpriteComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

glm::vec2 SpriteComponent::GetSize()
{
    if (_Renderable->GetSprite())
        return _Renderable->GetSprite()->_Size;
    
    return glm::vec2(0,0);
}

void SpriteComponent::SetRenderPass(RenderPass renderPass)
{
    _Renderable->SetRenderPass(renderPass);
}

void SpriteComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void SpriteComponent::SetSprite(const std::string& sprite)
{
    _Renderable->SetSprite(Engine::Instance()->GetSpriteRenderer()->GetSprite(sprite));
}

void SpriteComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->SetTint(tint);
}

void SpriteComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void SpriteComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void SpriteComponent::UpdateTransform()
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    }
}
