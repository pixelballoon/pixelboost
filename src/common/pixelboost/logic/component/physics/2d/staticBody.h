#pragma once

#include "pixelboost/logic/component/physics/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
struct FixtureDefinition2D;
    
class StaticBody2DComponent : public PhysicsComponent
{
public:
    StaticBody2DComponent(Entity* parent, b2World* world, FixtureDefinition2D& definition);
    ~StaticBody2DComponent();
    
    virtual pb::Uid GetType();
    static pb::Uid GetStaticType();
    
private:
    b2Body* _Body;
    b2World* _World;
};
    
}
