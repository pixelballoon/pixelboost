#include "Box2D/Box2D.h"

#include "pixelboost/logic/component/physics/2d/staticBody.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/physics/2d/helpers.h"

using namespace pb;

StaticBody2DComponent::StaticBody2DComponent(Entity* parent, b2World* world, BodyType type, glm::vec2 size)
    : PhysicsComponent(parent)
    , _World(world)
{
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(position.x, position.y);
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;
    
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    
    switch (type)
    {
        case kBodyTypeCircle:
        {
            b2CircleShape circle;
            circle.m_radius = size.x;
            fixtureDef.shape = &circle;
            break;
        }
            
        case kBodyTypeRect:
        {
            b2PolygonShape rect;
            rect.SetAsBox(size.x, size.y);
            fixtureDef.shape = &rect;
            break;
        }
    }
    
    _Body = world->CreateBody(&bodyDef);
    _Body->CreateFixture(&fixtureDef);
}

StaticBody2DComponent::StaticBody2DComponent(Entity* parent, b2World* world, FixtureDefinition2D& definition)
    : PhysicsComponent(parent)
    , _World(world)
{
    pb::TransformComponent* transform = GetParent()->GetComponentByType<pb::TransformComponent>();
    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    
    _Body = PhysicsHelpers2D::CreateBodyFromDefinition(world, definition, glm::vec2(position.x,position.y), this, glm::vec2(scale.x, scale.y));
    _Body->SetTransform(b2Vec2(position.x, position.y), glm::radians(transform->GetRotation().z));
}

StaticBody2DComponent::~StaticBody2DComponent()
{
    _World->DestroyBody(_Body);
}

pb::Uid StaticBody2DComponent::GetType()
{
    return GetStaticType();
}

pb::Uid StaticBody2DComponent::GetStaticType()
{
    return TypeHash("staticBody2D");
}
