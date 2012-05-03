#pragma once

#include <string>

namespace pixelboost
{
    
class Record;

typedef unsigned int Uid;
    
class Struct
{
public:
    Struct(Uid uid, const std::string& type);
    virtual ~Struct();
    
    const std::string& GetType();
    Uid GetUid();
    
public:
    Uid _Uid;
    std::string _Type;
};
    
}
