#pragma once

#include "glm/glm.hpp"

#include "pixelboost/graphics/definitions.h"
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
        
        void SetMaterial(const std::string& filename, const std::string& pool = "default");
        std::shared_ptr<ResourceHandle<MaterialResource> > GetMaterial();
        
    private:
        void OnResourceLoaded(ResourceHandleBase* base, bool error);
        void OnResourceUnloading(ResourceHandleBase* base);
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
    private:
        glm::mat4x4 _LocalTransform;
        T* _Renderable;
        
        std::shared_ptr<ResourceHandle<MaterialResource> > _Material;
    };

}

#include "pixelboost/logic/component/graphics/renderable.inl"
