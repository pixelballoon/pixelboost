#include "pixelboost/animation/timeline/curve.h"
#include "pixelboost/data/resources/svgResource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

TimelineCurve::TimelineCurve(Entity* entity, float length, const HermiteCurve2D* curve, bool orientToCurve, Easing easing)
    : TimelineCurve(entity->GetScene(), entity->GetUid(), length, curve, orientToCurve, easing)
{
    
}

TimelineCurve::TimelineCurve(Scene* scene, Uid entityId, float length, const HermiteCurve2D* curve, bool orientToCurve, Easing easing)
{
    _Scene = scene;
    _EntityId = entityId;
    _Length = length;
    _Curve = curve;
    _OrientToCurve = orientToCurve;
    _Easing = easing;
}

TimelineCurve::~TimelineCurve()
{
    
}

float TimelineCurve::GetLength()
{
    return _Length;
}

void TimelineCurve::SetCurve(const HermiteCurve2D* curve)
{
    _Curve = curve;
}
    
void TimelineCurve::OnUpdate(float time, float delta)
{
    if (_Curve && _Length > 0.f)
    {
        float t = _Easing.Evaluate(time/_Length) * _Curve->GetArcLength();
        glm::vec2 value = _Curve->EvaluateParam(t);
        _Scene->SendMessage(_EntityId, SetPositionMessage(glm::vec3(value, 0)));
        
        if (_OrientToCurve)
        {
            glm::vec2 direction = value - _Curve->EvaluateParam(t + 0.01f);
            float angle = glm::atan(direction.y, direction.x);
            
            _Scene->SendMessage(_EntityId, SetRotationMessage(glm::vec3(0,0,glm::degrees(angle))));
        }
    }
}

TimelineCurveSVG::TimelineCurveSVG(Entity* entity, float length, const std::string& resource, bool orientToCurve, Easing easing, const std::string& pool)
    : TimelineCurveSVG(entity->GetScene(), entity->GetUid(), length, resource, orientToCurve, easing, pool)
{
    
}

TimelineCurveSVG::TimelineCurveSVG(Scene* scene, Uid entityId, float length, const std::string& resource, bool orientToCurve, Easing easing, const std::string& pool)
    : TimelineCurve(scene, entityId, length, 0, orientToCurve, easing)
{
    _Resource = ResourceManager::Instance()->GetPool(pool)->GetResource<SvgResource>(resource);
    _Resource->SignalResourceLoaded.Connect(this, &TimelineCurveSVG::OnResourceLoaded);
    _Resource->SignalResourceUnloading.Connect(this, &TimelineCurveSVG::OnResourceUnloading);
    if (_Resource->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_Resource.get(), false);
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
        }
    }
}

void TimelineCurveSVG::OnResourceUnloading(Resource* resource)
{
    SetCurve(0);
}
