#include "pixelboost/graphics/components/sprite.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

SpriteComponent::SpriteComponent(Entity* parent, const std::string& sprite)
    : Component(parent)
    , _Sprite(sprite)
{
    parent->RegisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &SpriteComponent::OnRender));
}

SpriteComponent::~SpriteComponent()
{
    
}

Uid SpriteComponent::GetType()
{
    return TypeHash("sprite");
}

void SpriteComponent::OnRender(Uid sender, Message& message)
{
    
}
