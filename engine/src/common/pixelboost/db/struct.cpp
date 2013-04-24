#include "pixelboost/db/database.h"
#include "pixelboost/db/struct.h"

using namespace pb;

DbStructData::~DbStructData()
{
    
}
    
DbStruct::DbStruct(Uid uid, Uid type, DbStructData* data)
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

DbStructData* DbStruct::GetData() const
{
    return _Data;
}
