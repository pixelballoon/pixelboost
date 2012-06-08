#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;
    class FontRenderable;
    
    class FontComponent : public Component
    {
    public:
        FontComponent(Entity* parent, const std::string& font, const std::string& text);
        virtual ~FontComponent();
        
        Uid GetType();
        static Uid GetStaticType();
        
        void SetLayer(int layer);
        void SetAlignment(FontAlign alignment);
        void SetFont(const std::string& font);
        void SetText(const std::string& text);
        void SetTint(const glm::vec4& tint);
        void SetSize(float size);
        
        void SetLocalTransform(const glm::mat4x4& transform);
        
    private:
        void OnTransformChanged(Uid sender, Message& message);
        void UpdateTransform();
        
        glm::mat4x4 _LocalTransform;
        FontRenderable* _Renderable;
    };
    
}
