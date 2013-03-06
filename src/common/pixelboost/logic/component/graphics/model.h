#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component/graphics/renderable.h"

namespace pb
{
    
    class Message;
    class ModelRenderable;
    
    class ModelComponent : public RenderableComponent<ModelRenderable>
    {
        PB_DECLARE_COMPONENT
        
    protected:
        ModelComponent(Entity* parent);
        virtual ~ModelComponent();
        
    public:
        void SetShader(Shader* shader);
        void SetLayer(int layer);
        
        void SetModel(Model* model);
        Model* GetModel();
        
        void SetTexture(Texture* texture);
        Texture* GetTexture();
        
        void SetTint(const glm::vec4& tint);
        
        void SetAlphaBlend(bool alphaBlend);
    };
    
}
