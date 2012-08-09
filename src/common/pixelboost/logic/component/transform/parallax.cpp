#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform/parallax.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

ParallaxTransformComponent::ParallaxTransformComponent(Entity* parent, Uid parallaxEntityId, Uid parentTransform)
    : TransformComponent(parent, parentTransform)
    , _Dirty(true)
    , _ParallaxScale(1,1,1)
    , _Scale(1,1,1)
{
    Entity* parallaxEntity = GetScene()->GetEntityById(parallaxEntityId);
    
    if (parallaxEntity)
    {
        parallaxEntity->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &ParallaxTransformComponent::OnTransformChanged));
    }
}

ParallaxTransformComponent::~ParallaxTransformComponent()
{
    
}

Uid ParallaxTransformComponent::GetType()
{
    return GetStaticType();
}

Uid ParallaxTransformComponent::GetStaticType()
{
    return TypeHash("transform");
}

const glm::mat4x4& ParallaxTransformComponent::GetMatrix()
{
    if (_Dirty)
    {
        _Matrix = glm::translate(glm::mat4x4(), _Position + (_ParallaxPosition * _ParallaxScale));
        _Matrix = glm::scale(_Matrix, _Scale);
        _Matrix = glm::rotate(_Matrix, _Rotation.x, glm::vec3(1,0,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.y, glm::vec3(0,1,0));
        _Matrix = glm::rotate(_Matrix, _Rotation.z, glm::vec3(0,0,1));
    }
    
    return _Matrix;
}

void ParallaxTransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    
    OnChanged();
}

glm::vec3 ParallaxTransformComponent::GetPosition()
{
    return _Position;
}

void ParallaxTransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    OnChanged();
}

glm::vec3 ParallaxTransformComponent::GetRotation()
{
    return _Rotation;
}

void ParallaxTransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    
    OnChanged();
}

glm::vec3 ParallaxTransformComponent::GetScale()
{
    return _Scale;
}

void ParallaxTransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    OnChanged();
}

glm::vec3 ParallaxTransformComponent::GetParallaxScale()
{
    return _ParallaxScale;
}

void ParallaxTransformComponent::SetParallaxScale(const glm::vec3& parallaxScale)
{
    _ParallaxScale = parallaxScale;
}

void ParallaxTransformComponent::OnChanged()
{
    _Dirty = true;
    
    TransformChangedMessage message(GetParent(), this);
    GetScene()->SendMessage(GetParentUid(), message);
}

void ParallaxTransformComponent::OnTransformChanged(const Message& message)
{
    glm::vec3 position = static_cast<const TransformChangedMessage&>(message).GetEntity()->GetComponentByType<TransformComponent>()->GetPosition();
    _ParallaxPosition = glm::vec3(position.x, position.y, 0);
    
    OnChanged();
}
