#include "pixelboost/db/struct.h"

using namespace pixelboost;
    
Struct::Struct(Uid uid, Uid type, void* data)
    : _Uid(uid)
    , _Type(type)
    , _Data(data)
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

void* Struct::GetData() const
{
    return _Data;
}
