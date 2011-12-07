#include "pixelboost/db/entity.h"

namespace pixelboostdb
{
    
Entity::~Entity()
{
    
}

int Entity::GetType()
{
    return 1; // kDbEntity
}

void Entity::Deserialise(json::Object& container, Record* context)
{
    Struct::Deserialise(container, context);
    
    json::Object& transform = container["Transform"];
    json::Number& tx = transform["tx"];
    json::Number& ty = transform["ty"];
    json::Number& rz = transform["rz"];
    json::Number& sx = transform["sx"];
    json::Number& sy = transform["sy"];
    
    _Position = Vec2(tx.Value(), ty.Value());
    _Rotation = (float)rz.Value();
    _Scale = Vec2(sx.Value(), sy.Value());
}

}
