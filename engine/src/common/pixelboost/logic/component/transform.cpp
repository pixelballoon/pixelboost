#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::TransformComponent)

TransformComponent::TransformComponent(Entity* parent)
    : Component(parent)
    , _Dirty(kDirtyTypeThis)
    , _Scale(1,1,1)
{
    GetEntity()->RegisterMessageHandler<SetPositionMessage>(MessageHandler(this, &TransformComponent::OnSetPosition));
    GetEntity()->RegisterMessageHandler<SetRotationMessage>(MessageHandler(this, &TransformComponent::OnSetRotation));
    GetEntity()->RegisterMessageHandler<SetScaleMessage>(MessageHandler(this, &TransformComponent::OnSetScale));
}

TransformComponent::~TransformComponent()
{
    GetEntity()->UnregisterMessageHandler<SetPositionMessage>(MessageHandler(this, &TransformComponent::OnSetPosition));
    GetEntity()->UnregisterMessageHandler<SetRotationMessage>(MessageHandler(this, &TransformComponent::OnSetRotation));
    GetEntity()->UnregisterMessageHandler<SetScaleMessage>(MessageHandler(this, &TransformComponent::OnSetScale));
}

const glm::mat4x4& TransformComponent::GetMatrix()
{
    if (_Dirty != kDirtyTypeNone)
    {
        if (_Dirty == kDirtyTypeThis)
        {
            _LocalMatrix = CreateTransformMatrix(pb::kRotationOrder_XYZ, _Position, _Rotation, _Scale);
        }
        
        if (GetEntity()->GetParent())
        {
            pb::TransformComponent* parentTransform = GetEntity()->GetParent()->GetComponent<TransformComponent>();
            PbAssert(parentTransform && "One or more parents are missing a transform component");
            _Matrix = parentTransform->GetMatrix() * _LocalMatrix;
        }
    }
    
    if (GetEntity()->GetParent())
    {
        return _Matrix;
    }
    
    return _LocalMatrix;
}

void TransformComponent::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    _Position = position;
    _Rotation = rotation;
    _Scale = scale;
    
    Dirty(true);
}

glm::vec3 TransformComponent::GetPosition()
{
    return _Position;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    _Position = position;
    
    Dirty(true);
}

glm::vec3 TransformComponent::GetRotation()
{
    return _Rotation;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    _Rotation = rotation;
    
    Dirty(true);
}

glm::vec3 TransformComponent::GetScale()
{
    return _Scale;
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    _Scale = scale;
    
    Dirty(true);
}

glm::vec3 TransformComponent::GetWorldPosition()
{
    glm::vec4 position = _Matrix * glm::vec4(0,0,0,1);
    return glm::vec3(position.x, position.y, position.z);
}

void TransformComponent::Dirty(bool dirtyThis)
{
    if (_Dirty != kDirtyTypeThis)
        _Dirty = dirtyThis ? kDirtyTypeThis : kDirtyTypeParent;
    
    for (const auto& child : GetEntity()->GetChildren())
    {
        TransformComponent* transform = child->GetComponent<pb::TransformComponent>();
        if (transform)
        {
            transform->Dirty(false);
        }
    }
    
    TransformChangedMessage message(GetEntity(), this);
    GetScene()->SendMessage(GetEntityUid(), message);
}

void TransformComponent::OnSetPosition(const Message& message)
{
    auto positionMessage = message.As<SetPositionMessage>();
    
    SetPosition(positionMessage.GetPosition());
}

void TransformComponent::OnSetRotation(const Message& message)
{
    auto rotationMessage = message.As<SetRotationMessage>();
    
    SetRotation(rotationMessage.GetRotation());
}

void TransformComponent::OnSetScale(const Message& message)
{
    auto scaleMessage = message.As<SetScaleMessage>();
    
    SetScale(scaleMessage.GetScale());
}
