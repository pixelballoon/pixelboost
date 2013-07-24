#include "pixelboost/animation/timeline/curve.h"
#include "pixelboost/data/resources/svgResource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

TimelineCurve::TimelineCurve(Entity* entity, float length, const HermiteCurve2D* curve, Easing easing)
    : TimelineCurve(entity->GetScene(), entity->GetUid(), length, curve, easing)
{
    
}

TimelineCurve::TimelineCurve(Scene* scene, Uid entityId, float length, const HermiteCurve2D* curve, Easing easing)
{
    _Scene = scene;
    _EntityId = entityId;
    _Length = length;
    _Curve = curve;
    _FixedStep = false;
    _FixedStepDistance = 1.f;
    _OrientToCurve = true;
    _Easing = easing;
}

TimelineCurve::~TimelineCurve()
{
    
}

float TimelineCurve::GetLength()
{
    return _Length;
}

TimelineCurve* TimelineCurve::SetCurve(const HermiteCurve2D* curve)
{
    _Curve = curve;
    return this;
}

TimelineCurve* TimelineCurve::SetFixedStep(bool fixedStep, float distance)
{
    _FixedStep = fixedStep;
    _FixedStepDistance = distance;
    _PrevDistance = 0.f;
    return this;
}

TimelineCurve* TimelineCurve::SetOrientToCurve(bool orientToCurve)
{
    _OrientToCurve = orientToCurve;
    return this;
}

TimelineCurve* TimelineCurve::SetOffset(glm::vec2 offset)
{
    _Offset = offset;
    return this;
}

void TimelineCurve::OnUpdate(float time, float delta)
{
    if (_Curve && _Length > 0.f)
    {
        float t = _Easing.Evaluate(time/_Length) * _Curve->GetArcLength();
        
        if (_FixedStep)
        {
            while (_PrevDistance < t)
            {
                Evaluate(_PrevDistance);
                _PrevDistance += _FixedStepDistance;
            }
        } else {
            Evaluate(t);
        }
    }
}

void TimelineCurve::Evaluate(float t)
{
    glm::vec2 value = _Curve->EvaluateParam(t) + _Offset;
    _Scene->SendMessage(_EntityId, SetPositionMessage(glm::vec3(value, 0)));
    
    if (_OrientToCurve)
    {
        glm::vec2 direction = value - _Curve->EvaluateParam(t + 0.01f);
        float angle = glm::atan(direction.y, direction.x);
        
        _Scene->SendMessage(_EntityId, SetRotationMessage(glm::vec3(0,0,glm::degrees(angle))));
    }
}

TimelineCurveSVG::TimelineCurveSVG(Entity* entity, float length, const std::string& resource, Easing easing, const std::string& pool)
    : TimelineCurveSVG(entity->GetScene(), entity->GetUid(), length, resource, easing, pool)
{
    
}

TimelineCurveSVG::TimelineCurveSVG(Scene* scene, Uid entityId, float length, const std::string& resource, Easing easing, const std::string& pool)
    : TimelineCurve(scene, entityId, length, 0, easing)
{
    _Resource = ResourceManager::Instance()->GetPool(pool)->GetResource<SvgResource>(resource);
    _Resource->SignalResourceLoaded.Connect(this, &TimelineCurveSVG::OnResourceLoaded);
    _Resource->SignalResourceUnloading.Connect(this, &TimelineCurveSVG::OnResourceUnloading);
    if (_Resource->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Resource.get(), false);
    }
}

TimelineCurveSVG::~TimelineCurveSVG()
{
    if (_Resource)
    {
        _Resource->SignalResourceLoaded.Disconnect(this, &TimelineCurveSVG::OnResourceLoaded);
        _Resource->SignalResourceUnloading.Disconnect(this, &TimelineCurveSVG::OnResourceUnloading);
    }
}

void TimelineCurveSVG::OnResourceLoaded(Resource* resource, bool error)
{
    if (error)
    {
        SetCurve(0);
        return;
    }
    
    auto svgResource = static_cast<SvgResource*>(resource);
    
    for (const auto& group : svgResource->GetGroups())
    {
        for (const auto& path : group.second.Paths)
        {
            SetCurve(&path.Curve);
            return;
        }
    }
    
    PbLogError("pb.animation.timeline.curve", "No valid curves in SVG resource %s", resource->GetFilename().c_str());
}

void TimelineCurveSVG::OnResourceUnloading(Resource* resource)
{
    SetCurve(0);
}
