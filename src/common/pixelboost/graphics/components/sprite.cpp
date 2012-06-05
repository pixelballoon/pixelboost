#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/sprite.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

SpriteComponent::SpriteComponent(Entity* parent, const std::string& sprite)
    : Component(parent)
{
    _Renderable = new SpriteRenderable(parent->GetUid());
    _Renderable->Sprite = pb::Game::Instance()->GetSpriteRenderer()->GetSprite(sprite);
    
    parent->RegisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &SpriteComponent::OnRender));
}

SpriteComponent::~SpriteComponent()
{
    
}

Uid SpriteComponent::GetType()
{
    return TypeHash("sprite");
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

void SpriteComponent::OnRender(Uid sender, Message& message)
{
    Renderer::Instance()->AddItem(_Renderable);
}
