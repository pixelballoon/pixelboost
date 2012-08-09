#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/staticBody.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/transform.h"
#include "pixelboost/logic/system/physics/2d/physics.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/physics/2d/helpers.h"

using namespace pb;

StaticBody2DComponent::StaticBody2DComponent(Entity* parent, BodyType type, glm::vec2 size)
    : PhysicsComponent(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(position.x, position.y);
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;
    
    b2FixtureDef fixtureDef;
    
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
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &StaticBody2DComponent::OnTransformChanged));
}

StaticBody2DComponent::StaticBody2DComponent(Entity* parent, FixtureDefinition2D& definition)
    : PhysicsComponent(parent)
{
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    
    _Body = PhysicsHelpers2D::CreateBodyFromDefinition(world, definition, glm::vec2(position.x,position.y), this, glm::vec2(scale.x, scale.y));
    _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(transform->GetRotation().z));
    
    GetParent()->RegisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &StaticBody2DComponent::OnTransformChanged));
}

StaticBody2DComponent::~StaticBody2DComponent()
{
    GetParent()->UnregisterMessageHandler(TransformChangedMessage::GetStaticType(), Entity::MessageHandler(this, &StaticBody2DComponent::OnTransformChanged));
    
    pb::PhysicsSystem2D* physicsSystem = GetScene()->GetSystemByType<pb::PhysicsSystem2D>();
    
    b2World* world = physicsSystem->GetPhysicsWorld();
    
    world->DestroyBody(_Body);
}

pb::Uid StaticBody2DComponent::GetType()
{
    return GetStaticType();
}

pb::Uid StaticBody2DComponent::GetStaticType()
{
    return TypeHash("staticBody2D");
}

void StaticBody2DComponent::SetSensor(bool isSensor)
{
    b2Fixture* fixture = _Body->GetFixtureList();
    
    while (fixture)
    {
        fixture->SetSensor(isSensor);
        fixture = fixture->GetNext();
    }
}

void StaticBody2DComponent::OnTransformChanged(const Message& message)
{
    UpdateTransform();
}

void StaticBody2DComponent::UpdateTransform()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();    
    
    if (transform)
    {
        glm::vec3 position = transform->GetPosition();
        glm::vec3 rotation = transform->GetRotation();
        _Body->SetTransform(b2Vec2(position.x, position.y), rotation.z);
    }
}

#endif