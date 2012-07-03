#pragma once

#include <string>

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class DbStruct
{
public:
    DbStruct(Uid uid, Uid type, void* _Data);
    virtual ~DbStruct();
    
    Uid GetType() const;
    Uid GetUid() const;
    
    void* GetData() const;
    
public:
    Uid _Uid;
    Uid _Type;
    
    void* _Data;
};
    
}
