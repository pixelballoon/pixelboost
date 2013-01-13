#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/ellipse.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

EllipseComponent::EllipseComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableEllipse(parent->GetUid());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &EllipseComponent::OnTransformChanged));
    
    UpdateTransform();
}

EllipseComponent::~EllipseComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &EllipseComponent::OnTransformChanged));
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

Uid EllipseComponent::GetType()
{
    return EllipseComponent::GetStaticType();
}

Uid EllipseComponent::GetStaticType()
{
    return TypeHash("pb::EllipseComponent");
}

void EllipseComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void EllipseComponent::SetColor(glm::vec4 color)
{
    _Renderable->SetColor(color);
}

glm::vec4 EllipseComponent::GetColor()
{
    return _Renderable->GetColor();
}

void EllipseComponent::SetSize(glm::vec2 size)
{
    _Renderable->SetSize(size);
}

glm::vec2 EllipseComponent::GetSize()
{
    return _Renderable->GetSize();
}

void EllipseComponent::SetSolid(bool solid)
{
    _Renderable->SetSolid(solid);
}

bool EllipseComponent::GetSolid()
{
    return _Renderable->GetSolid();
}

void EllipseComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void EllipseComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void EllipseComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    } else {
        _Renderable->SetTransform(_LocalTransform);
    }
}
