#pragma once

#include <set>

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;

    class RectTouchComponent : public Component, public TouchHandler
    {
        PB_DECLARE_COMPONENT
        
    protected:
        RectTouchComponent(Entity* parent, bool debugRender = false);
        virtual ~RectTouchComponent();
        
    public:
        void SetLocalTransform(const glm::mat4x4& localTransform);
        const glm::mat4x4& GetLocalTransform();
        
        void SetSize(const glm::vec2& size);
        void SetCaptureEvents(bool captureEvents);
        void SetMultiTouch(bool multiTouch);
        
    private:
        glm::vec3 GetPosition();
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnTouchDown(TouchEvent touch);
        virtual bool OnTouchMove(TouchEvent touch);
        virtual bool OnTouchUp(TouchEvent touch);
        
        void OnDebugRender(const pb::Message& message);
        
        bool AddTouch(TouchEvent touch, glm::vec2 position);
        void RemoveTouch(TouchEvent touch);
        bool HasTouch(TouchEvent touch);
        
        glm::vec2 _Size;
        glm::mat4x4 _LocalTransform;
        
        bool _CaptureEvents;
        bool _MultiTouch;
        std::map<int, glm::vec2> _Touches;
        
        bool _DebugRender;
    };

}
