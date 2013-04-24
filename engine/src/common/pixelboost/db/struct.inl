#include "pixelboost/db/struct.h"

namespace pb
{

template <typename T> const T* DbStructData::As() const
{
    return IsA<T>() ? static_cast<const T*>(this) : 0;
}

template <typename T> T* DbStructData::As()
{
    return IsA<T>() ? static_cast<T*>(this) : 0;
}

template <typename T> bool DbStructData::IsA() const
{
    return IsA(T::GetStaticType());
}

template <typename T> const T* DbStruct::GetData() const
{
    return _Data ? _Data->As<T>() : 0;
}

}
