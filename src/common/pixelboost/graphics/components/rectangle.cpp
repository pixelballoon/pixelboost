#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/rectangle.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

RectangleComponent::RectangleComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableRectangle(parent->GetUid());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &RectangleComponent::OnTransformChanged));
    
    UpdateTransform();
}

RectangleComponent::~RectangleComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &RectangleComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid RectangleComponent::GetType()
{
    return TypeHash("rectangle");
}

void RectangleComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void RectangleComponent::SetColor(glm::vec4 color)
{
    _Renderable->Color = color;
}

glm::vec4 RectangleComponent::GetColor()
{
    return _Renderable->Color;
}

void RectangleComponent::SetSize(glm::vec2 size)
{
    _Renderable->Size = size;
}

glm::vec2 RectangleComponent::GetSize()
{
    return _Renderable->Size;
}

void RectangleComponent::SetSolid(bool solid)
{
    _Renderable->Solid = solid;
}

bool RectangleComponent::GetSolid()
{
    return _Renderable->Solid;
}

void RectangleComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void RectangleComponent::OnTransformChanged(Uid sender, const Message& message)
{
    UpdateTransform();
}

void RectangleComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->Transform = transform->GetMatrix() * _LocalTransform;
    }
}
