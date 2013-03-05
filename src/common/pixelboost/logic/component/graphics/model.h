#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;
    class ModelRenderable;
    
    class ModelComponent : public Component
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
        
        void SetLocalTransform(const glm::mat4x4& transform);
        
        const ModelRenderable* GetRenderable() const;
        
    private:
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
        glm::mat4x4 _LocalTransform;
        ModelRenderable* _Renderable;
    };
    
}
