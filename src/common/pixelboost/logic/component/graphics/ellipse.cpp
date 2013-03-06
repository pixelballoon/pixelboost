#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/ellipse.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::EllipseComponent)

EllipseComponent::EllipseComponent(Entity* parent)
    : RenderableComponent<pb::PrimitiveRenderableEllipse>(parent)
{
    
}

EllipseComponent::~EllipseComponent()
{
    
}

void EllipseComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void EllipseComponent::SetColor(glm::vec4 color)
{
    GetRenderable()->SetColor(color);
}

glm::vec4 EllipseComponent::GetColor()
{
    return GetRenderable()->GetColor();
}

void EllipseComponent::SetSize(glm::vec2 size)
{
    GetRenderable()->SetSize(size);
}

glm::vec2 EllipseComponent::GetSize()
{
    return GetRenderable()->GetSize();
}

void EllipseComponent::SetSolid(bool solid)
{
    GetRenderable()->SetSolid(solid);
}

bool EllipseComponent::GetSolid()
{
    return GetRenderable()->GetSolid();
}
