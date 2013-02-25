#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/logic/component/graphics/camera.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::CameraComponent)

CameraComponent::CameraComponent(Entity* parent, Camera* camera)
    : Component(parent)
{
    _Camera = camera;
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    UpdateTransform();
}

CameraComponent::~CameraComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    delete _Camera;
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
