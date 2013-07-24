#pragma once

#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/animation/easing/ease.h"
#include "pixelboost/animation/timeline/element.h"
#include "pixelboost/framework/definitions.h"
#include "pixelboost/maths/hermiteCurve.h"

namespace pb
{
    
    class Entity;
    class Resource;
    class Scene;
    class SvgResource;
    
    class TimelineCurve : public TimelineElement
    {
    public:
        TimelineCurve(Entity* entity, float length, const HermiteCurve2D* curve, Easing easing = Easing::Linear());
        TimelineCurve(Scene* scene, Uid entityId, float length, const HermiteCurve2D* curve, Easing easing = Easing::Linear());
        ~TimelineCurve();
        
        virtual float GetLength();
        
        TimelineCurve* SetCurve(const HermiteCurve2D* curve);
        TimelineCurve* SetFixedStep(bool fixedStep, float distance = 1.f);
        TimelineCurve* SetOrientToCurve(bool orientToCurve);
        TimelineCurve* SetOffset(glm::vec2 offset);
        
    private:
        virtual void OnUpdate(float time, float delta);
        
        void Evaluate(float t);
        
        Scene* _Scene;
        Uid _EntityId;
        float _Length;
        
        const HermiteCurve2D* _Curve;
        bool _FixedStep;
        float _FixedStepDistance;
        bool _OrientToCurve;
        glm::vec2 _Offset;
        
        float _PrevDistance;
        
        Easing _Easing;
    };
    
    class TimelineCurveSVG : public TimelineCurve
    {
    public:
        TimelineCurveSVG(Entity* entity, float length, const std::string& resource, Easing easing = Easing::Linear(), const std::string& pool = "default");
        TimelineCurveSVG(Scene* scene, Uid entityId, float length, const std::string& resource, Easing easing = Easing::Linear(), const std::string& pool = "default");
        ~TimelineCurveSVG();
        
    private:
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        std::shared_ptr<pb::SvgResource> _Resource;
    };

}
