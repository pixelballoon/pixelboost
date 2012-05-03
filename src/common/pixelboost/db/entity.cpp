#include "pixelboost/db/entity.h"

using namespace pixelboost;
    
Entity::Entity(Uid uid, const std::string& type, void* data)
    : Struct(uid, type)
    , _Data(data)
{
    
}
    
Entity::~Entity()
{
    
}
    
void* Entity::GetData()
{
    return _Data;
}
