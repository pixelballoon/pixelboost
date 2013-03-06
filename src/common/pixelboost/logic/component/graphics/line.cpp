#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/line.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::LineComponent)

LineComponent::LineComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableLine(parent->GetUid());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
}

LineComponent::~LineComponent()
{
    GetScene()->GetSystemByType<pb::RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

void LineComponent::SetColor(glm::vec4 color)
{
    _Renderable->SetColor(color);
}

glm::vec4 LineComponent::GetColor()
{
    return _Renderable->GetColor();
}

void LineComponent::SetLine(glm::vec3 start, glm::vec3 end)
{
    _Renderable->SetStart(start);
    _Renderable->SetEnd(end);
}

void LineComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void LineComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void LineComponent::UpdateTransform()
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    } else {
        _Renderable->SetTransform(_LocalTransform);
    }
}
