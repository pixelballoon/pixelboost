#pragma once

#include "pixelboost/db/struct.h"
#include "pixelboost/math/maths.h"

#include <string>

namespace pb
{
    
class Record;
    
class Entity : public Struct
{
public:
    Entity(Uid uid, Uid type, void* data);
    virtual ~Entity();
    
    const Vec3& GetPosition() const;
    const Vec3& GetRotation() const;
    const Vec3& GetScale() const;
    
private:
    Vec3 _Position;
    Vec3 _Rotation;
    Vec3 _Scale;
};
    
}
