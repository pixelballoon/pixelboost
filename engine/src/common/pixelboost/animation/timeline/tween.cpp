#include "pixelboost/animation/timeline/tween.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/message/color.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

TimelineTweenPosition::TimelineTweenPosition(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(entity, length, from, to, easing)
{
    
}

TimelineTweenPosition::TimelineTweenPosition(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(scene, entityId, length, from, to, easing)
{
    
}

void TimelineTweenPosition::OnTweenChanged(glm::vec3 value)
{
    _Scene->SendMessage(_EntityId, SetPositionMessage(value));
}

TimelineTweenRotate::TimelineTweenRotate(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(entity, length, from, to, easing)
{
    
}

TimelineTweenRotate::TimelineTweenRotate(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(scene, entityId, length, from, to, easing)
{
    
}

void TimelineTweenRotate::OnTweenChanged(glm::vec3 value)
{
    _Scene->SendMessage(_EntityId, SetRotationMessage(value));
}

TimelineTweenScale::TimelineTweenScale(Entity* entity, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(entity, length, from, to, easing)
{
    
}

TimelineTweenScale::TimelineTweenScale(Scene* scene, Uid entityId, float length, glm::vec3 from, glm::vec3 to, Easing easing)
    : TimelineTween(scene, entityId, length, from, to, easing)
{
    
}

void TimelineTweenScale::OnTweenChanged(glm::vec3 value)
{
    _Scene->SendMessage(_EntityId, SetScaleMessage(value));
}

TimelineTweenColor::TimelineTweenColor(Entity* entity, float length, glm::vec4 from, glm::vec4 to, Easing easing)
    : TimelineTween(entity, length, from, to, easing)
{
    
}

TimelineTweenColor::TimelineTweenColor(Scene* scene, Uid entityId, float length, glm::vec4 from, glm::vec4 to, Easing easing)
    : TimelineTween(scene, entityId, length, from, to, easing)
{
    
}

void TimelineTweenColor::OnTweenChanged(glm::vec4 value)
{
    _Scene->SendMessage(_EntityId, SetColorMessage(value));
}
