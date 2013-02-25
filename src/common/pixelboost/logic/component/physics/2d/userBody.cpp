#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/userBody.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/physics/2d/physics.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/physics/2d/helpers.h"

using namespace pb;

PB_DEFINE_COMPONENT_DERIVED(pb::PhysicsUserBody2DComponent, pb::PhysicsComponent2D)

PhysicsUserBody2DComponent::PhysicsUserBody2DComponent(Entity* parent, BodyType type, BodyShape shape, glm::vec2 size)
    : PhysicsComponent2D(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(position.x, position.y);
    switch (type)
    {
        case kBodyTypeDynamic:
            bodyDef.type = b2_dynamicBody;
            break;
        case kBodyTypeStatic:
            bodyDef.type = b2_staticBody;
            break;
        case kBodyTypeKinematic:
            bodyDef.type = b2_kinematicBody;
            break;
    }
    bodyDef.userData = this;
    
    b2FixtureDef fixtureDef;
    
    b2CircleShape circle;
    b2PolygonShape rect;
    
    switch (shape)
    {
        case kBodyShapeCircle:
        {
            circle.m_radius = size.x/2.f;
            fixtureDef.shape = &circle;
            break;
        }
            
        case kBodyShapeRect:
        {
            rect.SetAsBox(size.x/2.f, size.y/2.f);
            fixtureDef.shape = &rect;
            break;
        }
    }
    
    _Body = world->CreateBody(&bodyDef);
    _Body->CreateFixture(&fixtureDef);
    _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(transform->GetRotation().z));
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &PhysicsUserBody2DComponent::OnTransformChanged));
}

PhysicsUserBody2DComponent::PhysicsUserBody2DComponent(Entity* parent, BodyType type, FixtureDefinition2D& fixtureDefinition)
    : PhysicsComponent2D(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    
    b2BodyDef bodyDefinition;
    switch (type)
    {
        case kBodyTypeDynamic:
            bodyDefinition.type = b2_dynamicBody;
            break;
        case kBodyTypeStatic:
            bodyDefinition.type = b2_staticBody;
            break;
        case kBodyTypeKinematic:
            bodyDefinition.type = b2_kinematicBody;
            break;
    }
    bodyDefinition.position = b2Vec2(position.x, position.y);
    bodyDefinition.userData = this;
    
    _Body = PhysicsHelpers2D::CreateBodyFromDefinition(world, bodyDefinition, fixtureDefinition, 1.f, glm::vec2(scale.x, scale.y));
    _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(transform->GetRotation().z));
    
    GetParent()->RegisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &PhysicsUserBody2DComponent::OnTransformChanged));
}

PhysicsUserBody2DComponent::~PhysicsUserBody2DComponent()
{
    GetParent()->UnregisterMessageHandler<TransformChangedMessage>(MessageHandler(this, &PhysicsUserBody2DComponent::OnTransformChanged));
    
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    world->DestroyBody(_Body);
}

void PhysicsUserBody2DComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void PhysicsUserBody2DComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        glm::vec3 position = transform->GetPosition();
        glm::vec3 rotation = transform->GetRotation();
        _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(rotation.z));
    }
}

#endif
