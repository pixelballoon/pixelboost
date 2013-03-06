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
    : RenderableComponent<pb::SpriteRenderable>(parent)
{
    
}

SpriteComponent::~SpriteComponent()
{
    
}

glm::vec2 SpriteComponent::GetSize()
{
    if (GetRenderable()->GetSprite())
        return GetRenderable()->GetSprite()->_Size;
    
    return glm::vec2(0,0);
}

void SpriteComponent::SetRenderPass(RenderPass renderPass)
{
    GetRenderable()->SetRenderPass(renderPass);
}

void SpriteComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void SpriteComponent::SetSprite(const std::string& sprite)
{
    GetRenderable()->SetSprite(Engine::Instance()->GetSpriteRenderer()->GetSprite(sprite));
}

void SpriteComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
}
