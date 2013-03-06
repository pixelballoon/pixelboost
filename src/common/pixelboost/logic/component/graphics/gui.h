#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component.h"

namespace pb
{
    
    class GuiRenderable;
    class Message;
    
    class GuiComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        GuiComponent(Entity* parent);
        virtual ~GuiComponent();
        
    public:        
        void SetLocalTransform(const glm::mat4x4& localTransform);
        
    private:
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
    private:
        glm::mat4x4 _LocalTransform;
        GuiRenderable* _Renderable;
    };
    
}
