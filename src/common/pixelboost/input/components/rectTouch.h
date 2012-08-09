#pragma once

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;

    class RectTouchComponent : public Component, TouchHandler
    {
    public:
        RectTouchComponent(Entity* parent, bool debugRender = false);
        virtual ~RectTouchComponent();
        
        Uid GetStaticType();
        virtual Uid GetType();
        
        void SetSize(const glm::vec2& size);
        
    private:
        glm::vec3 GetPosition();
        
        virtual bool OnTouchDown(Touch touch);
        virtual bool OnTouchMove(Touch touch);
        virtual bool OnTouchUp(Touch touch);
        
        void OnDebugRender(const pb::Message& message);
        
        glm::vec2 _Size;
        
        int _TouchId;
        glm::vec2 _TouchPosition;
        
        bool _DebugRender;
    };

}
