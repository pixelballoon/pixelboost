#pragma once

#include "pixelboost/logic/component.h"

class b2Body;
class b2World;

namespace pb
{
    
struct FixtureDefinition2D;

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(Entity* parent);
    ~PhysicsComponent();
    
    void SetUserData(void* userData);
    void* GetUserData();
    
private:
    void* _UserData;
};
    
}
