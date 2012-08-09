#include "Box2D/Box2D.h"

#include "pixelboost/graphics/renderer/box2d/box2dRenderer.h"
#include "pixelboost/logic/system/physics/2d/physics.h"

using namespace pb;

PhysicsSystem2D::PhysicsSystem2D(glm::vec2 gravity)
    : _DebugRender(false)
    , _PositionIterations(3)
    , _VelocityIterations(3)
{
    _World = new b2World(b2Vec2(gravity.x, gravity.y));
    
    _DebugRenderer = new Box2dRenderer();
    _World->SetDebugDraw(_DebugRenderer);
}

PhysicsSystem2D::~PhysicsSystem2D()
{
    delete _World;
}

pb::Uid PhysicsSystem2D::GetType()
{
    return PhysicsSystem2D::GetStaticType();
}

pb::Uid PhysicsSystem2D::GetStaticType()
{
    return pb::TypeHash("pb::PhysicsSystem2D");
}

void PhysicsSystem2D::Update(Scene* scene, float time)
{
    _World->Step(time, _VelocityIterations, _PositionIterations);
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

void PhysicsSystem2D::SetDebugRender(bool debugRender, int layer)
{
    _DebugRender = debugRender;
    _DebugRenderer->SetLayer(layer);
}
