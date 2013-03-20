#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Camera;
    class Message;
    class FontRenderable;
    
    class CameraComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        CameraComponent(Entity* entity);
        virtual ~CameraComponent();
        
    public:
        void SetCamera(Camera* camera);
                
    private:
        void OnTransformChanged(const Message& message);
        void UpdateTransform();
        
        Camera* _Camera;
    };
    
}
