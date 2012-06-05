#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/rectangle.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

RectangleComponent::RectangleComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableRectangle(parent->GetUid());
    
    parent->RegisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &RectangleComponent::OnRender));
}

RectangleComponent::~RectangleComponent()
{
    GetParent()->UnregisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &RectangleComponent::OnRender));
    
    delete _Renderable;
}

Uid RectangleComponent::GetType()
{
    return TypeHash("rectangle");
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

void RectangleComponent::OnRender(Uid sender, Message& message)
{
    pb::Renderer::Instance()->AddItem(_Renderable);
}
