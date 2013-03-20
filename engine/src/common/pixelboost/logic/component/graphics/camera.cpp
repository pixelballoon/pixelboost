#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/logic/component/graphics/camera.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::CameraComponent)

CameraComponent::CameraComponent(Entity* parent)
    : Component(parent)
{
    _Camera = 0;
    
    GetEntity()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    UpdateTransform();
}

CameraComponent::~CameraComponent()
{
    GetEntity()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    delete _Camera;
}

void CameraComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void CameraComponent::SetCamera(Camera* camera)
{
    _Camera = camera;
}

void CameraComponent::UpdateTransform()
{
    if (!_Camera)
        return;
    
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();    
    
    if (!transform)
        return;

    _Camera->Position = transform->GetPosition();
    _Camera->Rotation = transform->GetRotation();
}
