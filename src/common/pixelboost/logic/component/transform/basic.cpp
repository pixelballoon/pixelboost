#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/logic/component/transform/basic.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

PB_DEFINE_COMPONENT_DERIVED(pb::BasicTransformComponent, pb::TransformComponent)

BasicTransformComponent::BasicTransformComponent(Entity* parent)
    : TransformComponent(parent)
    , _Dirty(true)
    , _Scale(1,1,1)
{
    
}

BasicTransformComponent::~BasicTransformComponent()
{
    
}

const glm::mat4x4& BasicTransformComponent::GetMatrix()
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

void BasicTransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    
    Dirty();
}

glm::vec3 BasicTransformComponent::GetPosition()
{
    return _Position;
}

void BasicTransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    Dirty();
}

glm::vec3 BasicTransformComponent::GetRotation()
{
    return _Rotation;
}

void BasicTransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    
    Dirty();
}

glm::vec3 BasicTransformComponent::GetScale()
{
    return _Scale;
}

void BasicTransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    Dirty();
}

void BasicTransformComponent::Dirty()
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
