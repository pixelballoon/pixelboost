#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TransformComponent)

TransformComponent::TransformComponent(Entity* parent)
    : Component(parent)
    , _Dirty(true)
    , _Scale(1,1,1)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

const glm::mat4x4& TransformComponent::GetMatrix()
{
    if (_Dirty)
    {
        _Matrix = glm::mat4x4();
        
        if (GetEntity()->GetParent())
        {
            _Matrix = GetEntity()->GetParent()->GetComponent<TransformComponent>()->GetMatrix();
        }
        
        _Matrix = CreateTransformMatrix(pb::kRotationOrder_XYZ, _Position, _Rotation, _Scale, _Matrix);
    }
    
    return _Matrix;
}

void TransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    
    Dirty();
}

glm::vec3 TransformComponent::GetPosition()
{
    return _Position;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    Dirty();
}

glm::vec3 TransformComponent::GetRotation()
{
    return _Rotation;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    
    Dirty();
}

glm::vec3 TransformComponent::GetScale()
{
    return _Scale;
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    Dirty();
}

void TransformComponent::Dirty()
{
    _Dirty = true;
    
    for (const auto& child : GetEntity()->GetChildren())
    {
        TransformComponent* transform = child->GetComponent<pb::TransformComponent>();
        if (transform)
        {
            transform->Dirty();
        }
    }
    
    TransformChangedMessage message(GetEntity(), this);
    GetScene()->SendMessage(GetEntityUid(), message);
}
