#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/physics/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
    struct FixtureDefinition2D;
    class Message;
    
    class DynamicBody2DComponent : public PhysicsComponent
    {
    public:
        enum BodyType
        {
            kBodyTypeCircle,
            kBodyTypeRect,
        };
        
    public:
        DynamicBody2DComponent(Entity* parent, BodyType type, glm::vec2 size);
        DynamicBody2DComponent(Entity* parent, FixtureDefinition2D& fixtureDefinition);
        ~DynamicBody2DComponent();
        
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
