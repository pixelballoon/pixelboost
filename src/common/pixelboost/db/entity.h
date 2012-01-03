#pragma once

#include "pixelboost/db/struct.h"
#include "pixelboost/math/maths.h"

namespace pixelboostdb
{
    
class Record;
    
class Entity : public Struct
{
public:
    virtual ~Entity();
    
    virtual int GetType();
    virtual void Deserialise(json::Object& container, Record* context);
    
public:
    Vec2 _Position;
    float _Rotation;
    Vec2 _Scale;
};
    
}
