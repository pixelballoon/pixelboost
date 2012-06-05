#include "glm/gtc/matrix_transform.hpp"

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
    return GetStaticType();
}

Uid TransformComponent::GetStaticType()
{
    return TypeHash("transform");
}

const glm::mat4x4& TransformComponent::GetMatrix()
{
    if (_Dirty)
    {
        _Matrix = glm::scale(glm::mat4x4(), _Scale);
        _Matrix = glm::rotate(_Matrix, _Rotation.x, glm::vec3(1,0,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.y, glm::vec3(0,1,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.z, glm::vec3(0,0,1));
        _Matrix = glm::translate(_Matrix, _Position);
    }
    
    return _Matrix;
}

void TransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    _Dirty = true;
}

const glm::vec3& TransformComponent::GetPosition()
{
    return _Position;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    _Dirty = true;
}

const glm::vec3& TransformComponent::GetRotation()
{
    return _Rotation;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    _Dirty = true;
}

const glm::vec3& TransformComponent::GetScale()
{
    return _Scale;
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    _Dirty = true;
}