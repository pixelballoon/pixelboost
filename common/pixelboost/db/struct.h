#pragma once

#include "pixelboost/json/reader.h"

namespace pixelboostdb
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
