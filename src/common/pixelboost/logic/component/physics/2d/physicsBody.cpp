#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/physicsBody.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/physics/2d/physics.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/physics/2d/helpers.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::PhysicsBody2DComponent)

PhysicsBody2DComponent::PhysicsBody2DComponent(Entity* entity)
    : PhysicsComponent2D(entity)
{
    
}

void PhysicsBody2DComponent::Initialise(BodyType type, BodyShape shape, glm::vec2 size)
{
    PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetEntity()->GetComponent<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    glm::vec3 rotation = transform->GetRotation();
    
    b2BodyDef bodyDefinition;
    bodyDefinition.fixedRotation = false;
    bodyDefinition.position = b2Vec2(position.x, position.y);
    bodyDefinition.angle = glm::radians(rotation.z);
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
    bodyDefinition.userData = this;
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    
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
    
    _Body = world->CreateBody(&bodyDefinition);
    _Body->CreateFixture(&fixtureDef);
    
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
}

void PhysicsBody2DComponent::Initialise(BodyType type, FixtureDefinition2D& fixtureDefinition)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetEntity()->GetComponent<pb::TransformComponent>();
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
    
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
}

PhysicsBody2DComponent::~PhysicsBody2DComponent()
{
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
    
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    world->DestroyBody(_Body);
}

void PhysicsBody2DComponent::OnUpdate(const Message& message)
{
    UpdateTransform();
}

void PhysicsBody2DComponent::UpdateTransform()
{
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        transform->SetPosition(glm::vec3(_Body->GetPosition().x, _Body->GetPosition().y, 0));
        transform->SetRotation(glm::vec3(0, 0, glm::degrees(_Body->GetAngle())));
    }
}

#endif
