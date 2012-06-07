#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
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
