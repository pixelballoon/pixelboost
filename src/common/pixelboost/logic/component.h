#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{

class Component
{
public:
    Component();
    virtual ~Component();
    
    virtual Uid GetType() = 0;
};

}
