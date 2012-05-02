#pragma once

#include "pixelboost/data/json/reader.h"

namespace pixelboost
{
    
class Record;

typedef unsigned int Uid;
    
class Struct
{
public:
    virtual ~Struct();
    
    virtual int GetType();
    virtual void Deserialise(json::Object& container, Record* context);
    
public:
    Uid _Uid;
};
    
}
