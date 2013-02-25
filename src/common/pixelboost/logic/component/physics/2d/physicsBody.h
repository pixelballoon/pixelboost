#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/physics/2d/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
    class FixtureDefinition2D;
    class Message;
    
    class PhysicsBody2DComponent : public PhysicsComponent2D
    {
        PB_DECLARE_COMPONENT
        
    public:
        enum BodyShape
        {
            kBodyShapeCircle,
            kBodyShapeRect,
        };
        
    public:
        PhysicsBody2DComponent(Entity* parent, BodyType type, BodyShape shape, glm::vec2 size);
        PhysicsBody2DComponent(Entity* parent, BodyType type, FixtureDefinition2D& fixtureDefinition);
        ~PhysicsBody2DComponent();
        
    private:
        void OnUpdate(const Message& message);
        void UpdateTransform();
    };
    
}
