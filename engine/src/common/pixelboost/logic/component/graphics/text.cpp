#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/message/color.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/text/textRenderer.h"
#include "pixelboost/logic/component/graphics/text.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/util/localisation/string.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TextComponent)

TextComponent::TextComponent(Entity* parent)
    : RenderableComponent<pb::TextRenderable>(parent)
{
    GetEntity()->RegisterMessageHandler<SetColorMessage>(MessageHandler(this, &TextComponent::OnSetColor));
}

TextComponent::~TextComponent()
{
    GetEntity()->UnregisterMessageHandler<SetColorMessage>(MessageHandler(this, &TextComponent::OnSetColor));
}

void TextComponent::SetRenderPass(RenderPass renderPass)
{
    GetRenderable()->SetRenderPass(renderPass);
}

void TextComponent::SetLayer(int layer)
{
    GetRenderable()->SetLayer(layer);
}

void TextComponent::SetAlignment(FontAlign alignment)
{
    GetRenderable()->SetAlignment(alignment);
}

void TextComponent::SetFont(const std::string& font)
{
    GetRenderable()->SetFont(font);
}

void TextComponent::SetText(const std::string& text)
{
    GetRenderable()->SetText(text);
}

void TextComponent::SetTint(const glm::vec4& tint)
{
    GetRenderable()->SetTint(tint);
}

void TextComponent::SetSize(float size)
{
    GetRenderable()->SetSize(size);
}

void TextComponent::OnSetColor(const Message& message)
{
    auto colorMessage = message.As<SetColorMessage>();

    GetRenderable()->SetTint(colorMessage.GetColor());
}
