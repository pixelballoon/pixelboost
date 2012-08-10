#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform/basic.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

BasicTransformComponent::BasicTransformComponent(Entity* parent, Uid parentTransform)
    : TransformComponent(parent, parentTransform)
    , _Dirty(true)
    , _Scale(1,1,1)
{
    
}

BasicTransformComponent::~BasicTransformComponent()
{
    
}

Uid BasicTransformComponent::GetType()
{
    return GetStaticType();
}

Uid BasicTransformComponent::GetStaticType()
{
    return TransformComponent::GetStaticType();
}

const glm::mat4x4& BasicTransformComponent::GetMatrix()
{
    if (_Dirty)
    {
        _Matrix = glm::translate(glm::mat4x4(), _Position);
        _Matrix = glm::scale(_Matrix, _Scale);
        _Matrix = glm::rotate(_Matrix, _Rotation.x, glm::vec3(1,0,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.y, glm::vec3(0,1,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.z, glm::vec3(0,0,1));
    }
    
    return _Matrix;
}

void BasicTransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    
    OnChanged();
}

glm::vec3 BasicTransformComponent::GetPosition()
{
    return _Position;
}

void BasicTransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    OnChanged();
}

glm::vec3 BasicTransformComponent::GetRotation()
{
    return _Rotation;
}

void BasicTransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    
    OnChanged();
}

glm::vec3 BasicTransformComponent::GetScale()
{
    return _Scale;
}

void BasicTransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    OnChanged();
}

void BasicTransformComponent::OnChanged()
{
    _Dirty = true;
    
    TransformChangedMessage message(GetParent(), this);
    GetScene()->SendMessage(GetParentUid(), message);
}
