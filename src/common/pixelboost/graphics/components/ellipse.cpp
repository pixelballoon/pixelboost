#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/components/ellipse.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

EllipseComponent::EllipseComponent(Entity* parent)
    : Component(parent)
{
    _Renderable = new PrimitiveRenderableEllipse(parent->GetUid());
    
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
}

EllipseComponent::~EllipseComponent()
{
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);
    
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
