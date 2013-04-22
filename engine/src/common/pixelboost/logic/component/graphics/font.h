#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class FontRenderable;
    
    class FontComponent : public RenderableComponent<FontRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        FontComponent(Entity* parent);
        virtual ~FontComponent();
        
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
