#pragma once

#include "pixelboost/db/struct.h"
#include "pixelboost/math/maths.h"

#include <string>

namespace pixelboost
{
    
class Record;
    
class Entity : public Struct
{
public:
    Entity(Uid uid, const std::string& type, void* data);
    virtual ~Entity();
    
    void* GetData();
    
private:
    void* _Data;
    
    Vec2 _Position;
    float _Rotation;
    Vec2 _Scale;
};
    
}
