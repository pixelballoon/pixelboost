#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2D/Box2D.h"

#include "pixelboost/graphics/renderer/box2d/box2dRenderer.h"
#include "pixelboost/logic/component/physics/physics.h"
#include "pixelboost/logic/message/physics/collision.h"
#include "pixelboost/logic/system/physics/2d/physics.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PhysicsSystem2D::PhysicsSystem2D(glm::vec2 gravity)
    : _DebugRender(false)
    , _PositionIterations(3)
    , _VelocityIterations(3)
{
    _World = new b2World(b2Vec2(gravity.x, gravity.y));
    
    _World->SetContactListener(this);
    
    _DebugRenderer = new Box2dRenderer();
    _World->SetDebugDraw(_DebugRenderer);
}

PhysicsSystem2D::~PhysicsSystem2D()
{
    delete _World;
}

pb::Uid PhysicsSystem2D::GetType() const
{
    return PhysicsSystem2D::GetStaticType();
}

pb::Uid PhysicsSystem2D::GetStaticType()
{
    return pb::TypeHash("pb::PhysicsSystem2D");
}

void PhysicsSystem2D::Update(Scene* scene, float totalTime, float gameTime)
{
    _World->Step(gameTime, _VelocityIterations, _PositionIterations);
}

void PhysicsSystem2D::Render(Scene* scene, Viewport* viewport, RenderPass renderPass)
{
    if (_DebugRender)
    {
        _DebugRenderer->SetScene(scene);
        _World->DrawDebugData();
    }
}

b2World* PhysicsSystem2D::GetPhysicsWorld()
{
    return _World;
}

void PhysicsSystem2D::SetDebugRender(bool debugRender, int layer, int flags)
{
    _DebugRender = debugRender;
    _DebugRenderer->SetLayer(layer);
    _DebugRenderer->SetFlags(flags);
}

void PhysicsSystem2D::BeginContact(b2Contact* contact)
{
    b2Body* a = contact->GetFixtureA()->GetBody();
    b2Body* b = contact->GetFixtureB()->GetBody();
    
    pb::PhysicsComponent* actorA = static_cast<pb::PhysicsComponent*>(a->GetUserData());
    pb::PhysicsComponent* actorB = static_cast<pb::PhysicsComponent*>(b->GetUserData());
    
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    
    glm::vec2 position(worldManifold.points[0].x, worldManifold.points[0].y);
    glm::vec2 normal(worldManifold.normal.x, worldManifold.normal.y);
    
    pb::Scene* scene = actorA->GetScene();
    
    scene->SendMessage(actorA->GetParentUid(), PhysicsCollisionMessage(actorB, position, normal));
    scene->SendMessage(actorB->GetParentUid(), PhysicsCollisionMessage(actorA, position, -normal));
}

void PhysicsSystem2D::EndContact(b2Contact* contact)
{
    
}

#endif