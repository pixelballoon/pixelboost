#pragma once

#include <string>

#include "pixelboost/db/definitions.h"

namespace pixelboost
{
    
class Record;
    
class Struct
{
public:
    Struct(Uid uid, Uid type, void* _Data);
    virtual ~Struct();
    
    Uid GetType() const;
    Uid GetUid() const;
    
    void* GetData() const;
    
public:
    Uid _Uid;
    Uid _Type;
    
    void* _Data;
};
    
}
