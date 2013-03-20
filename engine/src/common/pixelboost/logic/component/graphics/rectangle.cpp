#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::RectangleComponent)

RectangleComponent::RectangleComponent(Entity* parent)
    : RenderableComponent<PrimitiveRenderableRectangle>(parent)
{

}

RectangleComponent::~RectangleComponent()
{

}

void RectangleComponent::SetColor(glm::vec4 color)
{
    GetRenderable()->SetColor(color);
}

glm::vec4 RectangleComponent::GetColor()
{
    return GetRenderable()->GetColor();
}

void RectangleComponent::SetSize(glm::vec2 size)
{
    GetRenderable()->SetSize(size);
}

glm::vec2 RectangleComponent::GetSize()
{
    return GetRenderable()->GetSize();
}

void RectangleComponent::SetSolid(bool solid)
{
    GetRenderable()->SetSolid(solid);
}

bool RectangleComponent::GetSolid()
{
    return GetRenderable()->GetSolid();
}
