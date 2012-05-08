#pragma once

#include <string>

#include "pixelboost/db/definitions.h"

namespace pixelboost
{
    
class Record;
    
class Struct
{
public:
    Struct(Uid uid, Uid type);
    virtual ~Struct();
    
    Uid GetType() const;
    Uid GetUid() const;
    
public:
    Uid _Uid;
    Uid _Type;
};
    
}
