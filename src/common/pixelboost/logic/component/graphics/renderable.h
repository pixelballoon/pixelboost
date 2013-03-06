#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;

    template <class T> class RenderableComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        RenderableComponent(Entity* parent);
        virtual ~RenderableComponent();
        
    public:
        T* GetRenderable() const;
        
        RenderPass GetRenderPass() const;
        void SetRenderPass(RenderPass renderPass);
        
        int GetLayer() const;
        void SetLayer(int layer);
        
        const glm::mat4x4& GetLocalTransform() const;
        void SetLocalTransform(const glm::mat4x4& localTransform);
        
    private:
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
    private:
        glm::mat4x4 _LocalTransform;
        T* _Renderable;
    };

}

#include "pixelboost/logic/component/graphics/renderable.inl"
