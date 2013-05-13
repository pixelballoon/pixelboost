#pragma once

#include <set>

#include "glm/glm.hpp"

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/maths/plane.h"
#include "pixelboost/maths/ray.h"

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
        void SetIsUiComponent(bool isUiComponent);
        
        void SetPressDistance(float distance);
        
    private:
        Ray GetTouchRay(TouchEvent touch);
        Plane GetPlane();
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnTouchDown(TouchEvent touch);
        virtual bool OnTouchMove(TouchEvent touch);
        virtual bool OnTouchUp(TouchEvent touch);
        
        void OnDebugRender(const pb::Message& message);
        
        bool AddTouch(TouchEvent touch, glm::vec2 relativePosition);
        void RemoveTouch(TouchEvent touch);
        bool HasTouch(TouchEvent touch);
        
        glm::vec2 _Size;
        glm::mat4x4 _LocalTransform;
        
        bool _CaptureEvents;
        bool _MultiTouch;
        bool _IsUiComponent;
        
        struct TouchInfo
        {
            TouchInfo();
            TouchInfo(glm::vec2 startScreenPosition, glm::vec2 startRelativePosition);
            
            glm::vec2 StartScreenPosition;
            glm::vec2 StartRelativePosition;
            glm::vec2 CurrentScreenPosition;
            glm::vec2 CurrentRelativePosition;
        };
        
        std::map<int, TouchInfo> _Touches;
        
        bool _DebugRender;
        
        float _PressDistance;
    };

}
