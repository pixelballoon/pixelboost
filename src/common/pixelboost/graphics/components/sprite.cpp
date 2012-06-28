#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/sprite.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

SpriteComponent::SpriteComponent(Entity* parent, const std::string& sprite)
    : Component(parent)
{
    _Renderable = new SpriteRenderable(parent->GetUid());
    _Renderable->Sprite = pb::Game::Instance()->GetSpriteRenderer()->GetSprite(sprite);
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &SpriteComponent::OnTransformChanged));
    
    UpdateTransform();
}

SpriteComponent::~SpriteComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &SpriteComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid SpriteComponent::GetType()
{
    return GetStaticType();
}

Uid SpriteComponent::GetStaticType()
{
    return TypeHash("sprite");
}

glm::vec2 SpriteComponent::GetSize()
{
    if (_Renderable->Sprite)
        return _Renderable->Sprite->_Size;
    
    return glm::vec2(0,0);
}

void SpriteComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void SpriteComponent::SetSprite(const std::string& sprite)
{
    _Renderable->Sprite = pb::Game::Instance()->GetSpriteRenderer()->GetSprite(sprite);
}

void SpriteComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->Tint = tint;
}

void SpriteComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void SpriteComponent::OnTransformChanged(Uid sender, const Message& message)
{
    UpdateTransform();
}

void SpriteComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->Transform = transform->GetMatrix() * _LocalTransform;
    }
}
