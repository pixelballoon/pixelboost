#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component/graphics/font.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

FontComponent::FontComponent(Entity* parent, const std::string& font, const std::string& text)
    : Component(parent)
{
    _Renderable = new FontRenderable(parent->GetUid());
    _Renderable->Font = font;
    _Renderable->Text = text;
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &FontComponent::OnTransformChanged));
    
    UpdateTransform();
}

FontComponent::~FontComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &FontComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid FontComponent::GetType()
{
    return GetStaticType();
}

Uid FontComponent::GetStaticType()
{
    return TypeHash("font");
}

void FontComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void FontComponent::SetAlignment(FontAlign alignment)
{
    _Renderable->Alignment = alignment;
}

void FontComponent::SetFont(const std::string& font)
{
    _Renderable->Font = font;
}

void FontComponent::SetText(const std::string& text)
{
    _Renderable->Text = text;
}

void FontComponent::SetTint(const glm::vec4& tint)
{
    _Renderable->Tint = tint;
}

void FontComponent::SetSize(float size)
{
    _Renderable->Size = size;
}

void FontComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void FontComponent::OnTransformChanged(Uid sender, const Message& message)
{
    UpdateTransform();
}

void FontComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->Transform = transform->GetMatrix() * _LocalTransform;
    }
}