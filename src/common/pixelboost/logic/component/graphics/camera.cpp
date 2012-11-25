#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/logic/component/graphics/camera.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

CameraComponent::CameraComponent(Entity* parent, Camera* camera)
    : Component(parent)
{
    _Camera = camera;
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    UpdateTransform();
}

CameraComponent::~CameraComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(Entity::MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    delete _Camera;
}

Uid CameraComponent::GetType()
{
    return GetStaticType();
}

Uid CameraComponent::GetStaticType()
{
    return TypeHash("pb::CameraComponent");
}

void CameraComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void CameraComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        _Camera->Position = transform->GetPosition();
        _Camera->Rotation = transform->GetRotation();
    }
}
