#include "pixelboost/db/struct.h"

namespace pixelboost
{
    
Struct::~Struct()
{
    
}

int Struct::GetType()
{
    return 0; // kDbStruct
}

void Struct::Deserialise(json::Object& container, Record* context)
{
    json::Number& number = container["Uid"];
    
    _Uid = number.Value();
}

}
