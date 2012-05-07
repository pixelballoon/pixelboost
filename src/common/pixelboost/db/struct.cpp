#include "pixelboost/db/struct.h"

using namespace pixelboost;
    
Struct::Struct(Uid uid, Uid type)
    : _Uid(uid)
    , _Type(type)
{
    
}
    
Struct::~Struct()
{
    
}

Uid Struct::GetType() const
{
    return _Type;
}

Uid Struct::GetUid() const
{
    return _Uid;
}
