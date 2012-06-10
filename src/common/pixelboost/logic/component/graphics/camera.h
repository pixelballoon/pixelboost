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
    public:
        CameraComponent(Entity* parent, Camera* camera);
        virtual ~CameraComponent();
        
        Uid GetType();
        static Uid GetStaticType();
        
    private:
        void OnTransformChanged(Uid sender, const Message& message);
        void UpdateTransform();
        
        Camera* _Camera;
    };
    
}
