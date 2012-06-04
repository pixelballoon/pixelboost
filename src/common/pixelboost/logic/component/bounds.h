#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
class BoundsComponent : public Component
{
public:
    BoundsComponent();
    virtual ~BoundsComponent();
    
    Uid GetType();
};
    
}
