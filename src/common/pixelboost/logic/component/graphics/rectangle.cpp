#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
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
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RectangleComponent::OnTransformChanged));
    
    UpdateTransform();
}

RectangleComponent::~RectangleComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &RectangleComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid RectangleComponent::GetType()
{
    return RectangleComponent::GetStaticType();
}

Uid RectangleComponent::GetStaticType()
{
    return TypeHash("pb::RectangleComponent");
}

void RectangleComponent::SetRenderPass(RenderPass renderPass)
{
    _Renderable->SetRenderPass(renderPass);
}

void RectangleComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void RectangleComponent::SetColor(glm::vec4 color)
{
    _Renderable->SetColor(color);
}

glm::vec4 RectangleComponent::GetColor()
{
    return _Renderable->GetColor();
}

void RectangleComponent::SetSize(glm::vec2 size)
{
    _Renderable->SetSize(size);
}

glm::vec2 RectangleComponent::GetSize()
{
    return _Renderable->GetSize();
}

void RectangleComponent::SetSolid(bool solid)
{
    _Renderable->SetSolid(solid);
}

bool RectangleComponent::GetSolid()
{
    return _Renderable->GetSolid();
}

void RectangleComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void RectangleComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void RectangleComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    } else {
        _Renderable->SetTransform(_LocalTransform);
    }
}
