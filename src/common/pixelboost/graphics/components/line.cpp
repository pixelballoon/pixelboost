#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/line.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/message/render.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

LineComponent::LineComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableLine(parent->GetUid());
    
    parent->RegisterMessageHandler(RenderMessage::GetStaticType(), sigslot::Delegate2<Uid, Message&>(this, &LineComponent::OnRender));
}

LineComponent::~LineComponent()
{
    delete _Renderable;
}

Uid LineComponent::GetType()
{
    return TypeHash("rectangle");
}

void LineComponent::SetColor(glm::vec4 color)
{
    _Renderable->Color = color;
}

glm::vec4 LineComponent::GetColor()
{
    return _Renderable->Color;
}

void LineComponent::SetLine(glm::vec3 start, glm::vec3 end)
{
    _Renderable->Start = start;
    _Renderable->End = end;
}

void LineComponent::OnRender(Uid sender, Message& message)
{
    pb::Renderer::Instance()->AddItem(_Renderable);
}
