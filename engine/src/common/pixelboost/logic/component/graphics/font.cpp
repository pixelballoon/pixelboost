#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/message/color.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component/graphics/font.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/util/localisation/string.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::FontComponent)

FontComponent::FontComponent(Entity* parent)
    : RenderableComponent<pb::FontRenderable>(parent)
{
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &FontComponent::OnSetColor));
}

FontComponent::~FontComponent()
{
    GetEntity()->UnregisterMessageHandler<SetColorMessage>(MessageHandler(this, &FontComponent::OnSetColor));
}

void FontComponent::SetRenderPass(RenderPass renderPass)
{
    GetRenderable()->SetRenderPass(renderPass);
}

void FontComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void FontComponent::SetAlignment(FontAlign alignment)
{
    GetRenderable()->SetAlignment(alignment);
}

void FontComponent::SetFont(const std::string& font)
{
    GetRenderable()->SetFont(font);
}

void FontComponent::SetText(const std::string& text)
{
    GetRenderable()->SetText(text);
}

void FontComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
}

void FontComponent::SetSize(float size)
{
    GetRenderable()->SetSize(size);
}

void FontComponent::OnSetColor(const Message& message)
{
    auto colorMessage = message.As<SetColorMessage>();

    GetRenderable()->SetTint(colorMessage.GetColor());
}
