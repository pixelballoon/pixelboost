#include "pixelboost/db/struct.h"

using namespace pixelboost;
    
Struct::Struct(Uid uid, const std::string& type)
    : _Uid(uid)
    , _Type(type)
{
    
}
    
Struct::~Struct()
{
    
}

const std::string& Struct::GetType()
{
    return _Type;
}

Uid Struct::GetUid()
{
    return _Uid;
}
