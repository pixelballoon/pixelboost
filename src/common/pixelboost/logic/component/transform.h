#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{

class TransformComponent : public Component
{
public:
    TransformComponent(Entity* parent);
    virtual ~TransformComponent();
    
    Uid GetType();
    
private:
};

}
