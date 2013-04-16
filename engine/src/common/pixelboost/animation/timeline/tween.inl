#include "pixelboost/logic/entity.h"

namespace pb
{

template <typename T> TimelineTween<T>::TimelineTween(Entity* entity, float length, T from, T to, Easing easing)
    : TimelineTween(entity->GetScene(), entity->GetUid(), length, from, to, easing)
{

}

template <typename T> TimelineTween<T>::TimelineTween(Scene* scene, Uid entityId, float length, T from, T to, Easing easing)
{
    _IsPlaying = false;
    _Scene = scene;
    _EntityId = entityId;
    _Length = length;
    _From = from;
    _To = to;
    _Easing = easing;
}

template <typename T> TimelineTween<T>::~TimelineTween()
{

}
        
template <typename T> float TimelineTween<T>::GetLength()
{
    return _Length;
}

template <typename T> void TimelineTween<T>::OnUpdate(float time, float delta)
{
    OnTweenChanged(glm::mix(_From, _To, _Easing.Evaluate(time/_Length)));
}

}
