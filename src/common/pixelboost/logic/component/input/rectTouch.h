#pragma once

#include <set>

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
    class Message;

    class RectTouchComponent : public Component, public TouchHandler
    {
    public:
        RectTouchComponent(Entity* parent, bool debugRender = false);
        virtual ~RectTouchComponent();
        
        Uid GetStaticType();
        virtual Uid GetType();
        
        void SetSize(const glm::vec2& size);
        void SetMultiTouch(bool multiTouch);
        
    private:
        glm::vec3 GetPosition();
        
        virtual bool OnTouchDown(Touch touch);
        virtual bool OnTouchMove(Touch touch);
        virtual bool OnTouchUp(Touch touch);
        
        void OnDebugRender(const pb::Message& message);
        
        bool AddTouch(Touch touch, glm::vec2 position);
        void RemoveTouch(Touch touch);
        bool HasTouch(Touch touch);
        
        glm::vec2 _Size;
        
        bool _MultiTouch;
        std::map<int, glm::vec2> _Touches;
        
        bool _DebugRender;
    };

}
