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
    : RenderableComponent<PrimitiveRenderableLine>(parent)
{

}

LineComponent::~LineComponent()
{

}

void LineComponent::SetColor(glm::vec4 color)
{
    GetRenderable()->SetColor(color);
}

glm::vec4 LineComponent::GetColor()
{
    return GetRenderable()->GetColor();
}

void LineComponent::SetLine(glm::vec3 start, glm::vec3 end)
{
    GetRenderable()->SetStart(start);
    GetRenderable()->SetEnd(end);
}
