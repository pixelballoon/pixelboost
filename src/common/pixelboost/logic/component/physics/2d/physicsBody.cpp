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

PhysicsBody2DComponent::PhysicsBody2DComponent(Entity* parent, BodyType type, BodyShape shape, glm::vec2 size)
    : PhysicsComponent(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    
    b2BodyDef bodyDefinition;
    bodyDefinition.fixedRotation = false;
    bodyDefinition.position = b2Vec2(position.x, position.y);
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
            circle.m_radius = size.x;
            fixtureDef.shape = &circle;
            break;
        }
            
        case kBodyShapeRect:
        {
            rect.SetAsBox(size.x, size.y);
            fixtureDef.shape = &rect;
            break;
        }
    }
    
    _Body = world->CreateBody(&bodyDefinition);
    _Body->CreateFixture(&fixtureDef);
    
    GetParent()->RegisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
}

PhysicsBody2DComponent::PhysicsBody2DComponent(Entity* parent, BodyType type, FixtureDefinition2D& fixtureDefinition)
    : PhysicsComponent(parent)
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
    
    GetParent()->RegisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
}

PhysicsBody2DComponent::~PhysicsBody2DComponent()
{
    GetParent()->UnregisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &PhysicsBody2DComponent::OnUpdate));
    
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    world->DestroyBody(_Body);
}

Uid PhysicsBody2DComponent::GetType()
{
    return GetStaticType();
}

Uid PhysicsBody2DComponent::GetStaticType()
{
    return TypeHash("pb::PhysicsBody2DComponent");
}

b2Body* PhysicsBody2DComponent::GetBody()
{
    return _Body;
}

void PhysicsBody2DComponent::SetSensor(bool isSensor)
{
    b2Fixture* fixture = _Body->GetFixtureList();
    
    while (fixture)
    {
        fixture->SetSensor(isSensor);
        fixture = fixture->GetNext();
    }
}

void PhysicsBody2DComponent::OnUpdate(const Message& message)
{
    UpdateTransform();
}

void PhysicsBody2DComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();
    
    if (transform)
    {
        transform->SetPosition(glm::vec3(_Body->GetPosition().x, _Body->GetPosition().y, 0));
        transform->SetRotation(glm::vec3(0, 0, glm::degrees(_Body->GetTransform().q.GetAngle())));
    }
}

#endif
