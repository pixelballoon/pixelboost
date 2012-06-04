#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{

class TransformComponent : public Component
{
public:
    TransformComponent();
    virtual ~TransformComponent();
    
    Uid GetType();
    
private:
};

}
