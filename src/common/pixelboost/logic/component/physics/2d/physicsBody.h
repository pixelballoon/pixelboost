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
        PhysicsBody2DComponent(Entity* entity);
        ~PhysicsBody2DComponent();
        
        void Initialise(BodyType type, BodyShape shape, glm::vec2 size);
        void Initialise(BodyType type, FixtureDefinition2D& fixtureDefinition);
        
    private:
        void OnUpdate(const Message& message);
        void UpdateTransform();
    };
    
}
