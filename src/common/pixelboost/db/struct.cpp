#include "pixelboost/db/database.h"
#include "pixelboost/db/struct.h"

using namespace pb;
    
DbStruct::DbStruct(Uid uid, Uid type, void* data)
    : _Uid(uid)
    , _Type(type)
    , _Data(data)
{
    
}
    
DbStruct::~DbStruct()
{
    structDestroyed();
    Database::Instance()->Destroy(_Type, _Data);
}

Uid DbStruct::GetType() const
{
    return _Type;
}

Uid DbStruct::GetUid() const
{
    return _Uid;
}

void* DbStruct::GetData() const
{
    return _Data;
}
