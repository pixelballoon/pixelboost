#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/animation/easing/ease.h"
#include "pixelboost/animation/timeline/element.h"
#include "pixelboost/framework/definitions.h"

namespace pb
{
    
    class Entity;
    class Scene;
       
    template <typename T> class TimelineTween : public TimelineElement
    {
    public:
        TimelineTween(Entity* entity, float length, T from, T to, Easing easing = Easing::Linear());
        TimelineTween(Scene* scene, Uid entityId, float length, T from, T to, Easing easing = Easing::Linear());
        ~TimelineTween();
        
        virtual float GetLength();
        
    protected:
        virtual void OnTweenChanged(T value) = 0;
        
        Scene* _Scene;
        Uid _EntityId;
        
    private:
        virtual void OnUpdate(float time, float delta);
        
        float _Length;
        
        T _From;
        T _To;
        
        Easing _Easing;
    };
    
    class TimelineTweenPosition : public TimelineTween<glm::vec3>
    {
    public:
        TimelineTweenPosition(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());
        TimelineTweenPosition(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());

    protected:
        virtual void OnTweenChanged(glm::vec3 value);
    };
    
    class TimelineTweenRotate : public TimelineTween<glm::vec3>
    {
    public:
        TimelineTweenRotate(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());
        TimelineTweenRotate(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());
        
    protected:
        virtual void OnTweenChanged(glm::vec3 value);
    };
    
    class TimelineTweenScale : public TimelineTween<glm::vec3>
    {
    public:
        TimelineTweenScale(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());
        TimelineTweenScale(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing = Easing::Linear());
        
    protected:
        virtual void OnTweenChanged(glm::vec3 value);
    };
    
    class TimelineTweenColor : public TimelineTween<glm::vec4>
    {
    public:
        TimelineTweenColor(Entity* entity, float length, glm::vec4 from, glm::vec4 to, Easing easing = Easing::Linear());
        TimelineTweenColor(Scene* scene, Uid entityId, float length, glm::vec4 from, glm::vec4 to, Easing easing = Easing::Linear());
        
    protected:
        virtual void OnTweenChanged(glm::vec4 value);
    };

}

#include "pixelboost/animation/timeline/tween.inl"
