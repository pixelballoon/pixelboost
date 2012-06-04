#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
class BoundsComponent : public Component
{
public:
    BoundsComponent(Entity* parent);
    virtual ~BoundsComponent();
    
    Uid GetType();
};
    
}
