#ifndef LIBPIXEL__DB__ENTITY__H
#define LIBPIXEL__DB__ENTITY__H

#include "libpixel/db/struct.h"
#include "libpixel/math/maths.h"

namespace libpixeldb
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

#endif
