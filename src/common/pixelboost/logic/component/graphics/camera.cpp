#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/logic/component/graphics/camera.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/entity.h"
//#include "pixelboost/logic/scene.h"

using namespace pb;

CameraComponent::CameraComponent(Entity* parent, Camera* camera)
    : Component(parent)
{
    _Camera = camera;
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    UpdateTransform();
}

CameraComponent::~CameraComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &CameraComponent::OnTransformChanged));
    
    delete _Camera;
}

Uid CameraComponent::GetType()
{
    return GetStaticType();
}

Uid CameraComponent::GetStaticType()
{
    return TypeHash("camera");
}

void CameraComponent::OnTransformChanged(Uid sender, const Message& message)
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
