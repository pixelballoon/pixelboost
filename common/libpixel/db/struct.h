#pragma once

#include "libpixel/json/reader.h"

namespace libpixeldb
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
