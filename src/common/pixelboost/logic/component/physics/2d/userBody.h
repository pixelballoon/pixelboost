#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/physics/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
struct FixtureDefinition2D;
class Message;
    
class PhysicsUserBody2DComponent : public PhysicsComponent
{
public:
    enum BodyType
    {
        kBodyTypeStatic,
        kBodyTypeDynamic,
        kBodyTypeKinematic,
    };
    
    enum BodyShape
    {
        kBodyShapeCircle,
        kBodyShapeRect,
    };
    
public:
    PhysicsUserBody2DComponent(Entity* parent, BodyType type, BodyShape shape, glm::vec2 size);
    PhysicsUserBody2DComponent(Entity* parent, BodyType type, FixtureDefinition2D& definition);
    ~PhysicsUserBody2DComponent();
    
    virtual Uid GetType();
    static Uid GetStaticType();
    
    b2Body* GetBody();
    
    void SetSensor(bool isSensor);
    
private:
    void OnTransformChanged(const Message& message);
    void UpdateTransform();
    
    b2Body* _Body;
};
    
}
