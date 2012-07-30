#include "pixelboost/db/struct.h"

namespace pb
{

template<class T> const T* DbStruct::GetData() const
{
    return static_cast<T*>(_Data);
}

}