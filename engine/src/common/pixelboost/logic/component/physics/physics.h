#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
class PhysicsComponent : public Component
{
    PB_DECLARE_COMPONENT
    
protected:
    PhysicsComponent(Entity* parent);
    ~PhysicsComponent();
    
public:
    void SetUserData(void* userData);
    void* GetUserData();
    
private:
    void* _UserData;
};
    
}
