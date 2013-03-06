#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::GuiComponent)

GuiComponent::GuiComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new GuiRenderable(parent->GetUid());
    
    GetScene()->GetSystemByType<RenderSystem>()->AddItem(_Renderable);
}

GuiComponent::~GuiComponent()
{
    GetScene()->GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

void GuiComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
    
    UpdateTransform();
}

void GuiComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void GuiComponent::UpdateTransform()
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        _Renderable->SetTransform(transform->GetMatrix() * _LocalTransform);
    } else {
        _Renderable->SetTransform(_LocalTransform);
    }
}
