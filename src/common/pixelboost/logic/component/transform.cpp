#include "pixelboost/logic/component/transform.h"

using namespace pb;

TransformComponent::TransformComponent(Entity* parent)
    : Component(parent)
    , _Dirty(true)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

Uid TransformComponent::GetType()
{
    return TypeHash("transform");
}

const glm::mat4x4& TransformComponent::GetTransformMatrix()
{
    if (_Dirty)
    {
        // Recalculate matrix
    }
    
    return _Matrix;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    _Dirty = true;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    _Dirty = true;
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    _Dirty = true;
}
