#include "pixelboost/db/entity.h"

using namespace pixelboost;
    
Entity::Entity(Uid uid, Uid type, void* data)
    : Struct(uid, type)
    , _Data(data)
{
    
}
    
Entity::~Entity()
{
    
}
    
void* Entity::GetData() const
{
    return _Data;
}
