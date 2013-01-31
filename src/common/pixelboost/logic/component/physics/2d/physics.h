#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/component/physics/physics.h"

class b2Body;
class b2World;

namespace pb
{
    
    class PhysicsComponent2D : public PhysicsComponent
    {
    public:
        enum BodyType
        {
            kBodyTypeDynamic,
            kBodyTypeStatic,
            kBodyTypeKinematic,
        };
        
    public:
        PhysicsComponent2D(Entity* parent);
        ~PhysicsComponent2D();
        
        b2Body* GetBody();

        void SetSensor(bool isSensor);
        
    protected:
        b2Body* _Body;
    };
    
}
