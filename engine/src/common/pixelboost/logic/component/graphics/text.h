#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/text/textRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class TextRenderable;
    
    class TextComponent : public RenderableComponent<TextRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        TextComponent(Entity* parent);
        virtual ~TextComponent();
        
    public:
        void SetRenderPass(RenderPass renderPass);
        void SetLayer(int layer);
        void SetAlignment(FontAlign alignment);
        void SetFont(const std::string& font);
        void SetText(const std::string& text);
        void SetTint(const glm::vec4& tint);
        void SetSize(float size);

    private:
        void OnSetColor(const Message& message);
    };
    
}
