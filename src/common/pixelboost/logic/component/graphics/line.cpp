#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/line.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

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

Uid LineComponent::GetType()
{
    return TypeHash("rectangle");
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
