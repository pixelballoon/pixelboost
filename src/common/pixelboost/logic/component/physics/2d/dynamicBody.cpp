#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/dynamicBody.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/physics/2d/physics.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/physics/2d/helpers.h"

using namespace pb;

DynamicBody2DComponent::DynamicBody2DComponent(Entity* parent, BodyType type, glm::vec2 size)
    : PhysicsComponent(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = false;
    bodyDef.position = b2Vec2(position.x, position.y);
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = this;
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    
    b2CircleShape circle;
    b2PolygonShape rect;
    
    switch (type)
    {
        case kBodyTypeCircle:
        {
            circle.m_radius = size.x;
            fixtureDef.shape = &circle;
            break;
        }
            
        case kBodyTypeRect:
        {
            rect.SetAsBox(size.x, size.y);
            fixtureDef.shape = &rect;
            break;
        }
    }
    
    _Body = world->CreateBody(&bodyDef);
    _Body->CreateFixture(&fixtureDef);
    
    GetParent()->RegisterMessageHandler(UpdateMessage::GetStaticType(), Entity::MessageHandler(this, &DynamicBody2DComponent::OnUpdate));
}

DynamicBody2DComponent::DynamicBody2DComponent(Entity* parent, FixtureDefinition2D& fixtureDefinition)
    : PhysicsComponent(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    
    b2BodyDef bodyDefinition;
    bodyDefinition.type = b2_dynamicBody;
    bodyDefinition.position = b2Vec2(position.x, position.y);
    bodyDefinition.userData = this;
    
    _Body = PhysicsHelpers2D::CreateBodyFromDefinition(world, bodyDefinition, fixtureDefinition, 1.f, glm::vec2(scale.x, scale.y));
    _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(transform->GetRotation().z));
    
    GetParent()->RegisterMessageHandler(UpdateMessage::GetStaticType(), Entity::MessageHandler(this, &DynamicBody2DComponent::OnUpdate));
}

DynamicBody2DComponent::~DynamicBody2DComponent()
{
    GetParent()->UnregisterMessageHandler(UpdateMessage::GetStaticType(), Entity::MessageHandler(this, &DynamicBody2DComponent::OnUpdate));
    
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    world->DestroyBody(_Body);
}

Uid DynamicBody2DComponent::GetType()
{
    return GetStaticType();
}

Uid DynamicBody2DComponent::GetStaticType()
{
    return TypeHash("pb::DynamicBody2DComponent");
}

b2Body* DynamicBody2DComponent::GetBody()
{
    return _Body;
}

void DynamicBody2DComponent::SetSensor(bool isSensor)
{
    b2Fixture* fixture = _Body->GetFixtureList();
    
    while (fixture)
    {
        fixture->SetSensor(isSensor);
        fixture = fixture->GetNext();
    }
}

void DynamicBody2DComponent::OnUpdate(const Message& message)
{
    UpdateTransform();
}

void DynamicBody2DComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();
    
    if (transform)
    {
        transform->SetPosition(glm::vec3(_Body->GetPosition().x, _Body->GetPosition().y, 0));
        transform->SetRotation(glm::vec3(0, 0, glm::degrees(_Body->GetTransform().q.GetAngle())));
    }
}

#endif
