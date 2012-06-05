#include "pixelboost/db/definitions.h"

using namespace pb;

Uid pb::RuntimeTypeHash(std::string string)
{
    const size_t length = string.length()+1;
    Uid hash = 2166136261u;
    for (size_t i=0; i<length; ++i)
    {
        hash ^= string[i];
        hash *= 16777619u;
    }
    
    return hash;
}