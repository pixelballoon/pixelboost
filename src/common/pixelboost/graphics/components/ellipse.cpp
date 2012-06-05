#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/ellipse.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

EllipseComponent::EllipseComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableEllipse(parent->GetUid());
    
    parent->RegisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &EllipseComponent::OnRender));
}

EllipseComponent::~EllipseComponent()
{
    GetParent()->UnregisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &EllipseComponent::OnRender));
    
    delete _Renderable;
}

Uid EllipseComponent::GetType()
{
    return TypeHash("ellipse");
}

void EllipseComponent::SetColor(glm::vec4 color)
{
    _Renderable->Color = color;
}

glm::vec4 EllipseComponent::GetColor()
{
    return _Renderable->Color;
}

void EllipseComponent::SetSize(glm::vec2 size)
{
    _Renderable->Size = size;
}

glm::vec2 EllipseComponent::GetSize()
{
    return _Renderable->Size;
}

void EllipseComponent::OnRender(Uid sender, Message& message)
{
    pb::Renderer::Instance()->AddItem(_Renderable);
}
