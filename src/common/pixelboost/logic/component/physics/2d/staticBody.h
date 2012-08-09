#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/physics/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
struct FixtureDefinition2D;
class Message;
    
class StaticBody2DComponent : public PhysicsComponent
{
public:
    enum BodyType
    {
        kBodyTypeCircle,
        kBodyTypeRect,
    };
    
public:
    StaticBody2DComponent(Entity* parent, b2World* world, BodyType type, glm::vec2 size);
    StaticBody2DComponent(Entity* parent, b2World* world, FixtureDefinition2D& definition);
    ~StaticBody2DComponent();
    
    virtual pb::Uid GetType();
    static pb::Uid GetStaticType();
    
    void SetSensor(bool isSensor);
    
private:
    void OnTransformChanged(const Message& message);
    void UpdateTransform();
    
    b2Body* _Body;
    b2World* _World;
};
    
}
