#pragma once

#include <string>

#ifdef _MSC_VER
#define PB_INLINE __forceinline
#else
#define PB_INLINE inline
#endif

namespace pb
{
    typedef unsigned int Uid;
    
    template <unsigned int N, unsigned int I>
    struct FnvHash
    {
        PB_INLINE static unsigned int Hash(const char (&str)[N])
        {
            return (FnvHash<N, I-1>::Hash(str) ^ str[I-1])*16777619u;
        }
    };
    
    template <unsigned int N>
    struct FnvHash<N, 1>
    {
        PB_INLINE static unsigned int Hash(const char (&str)[N])
        {
            return (2166136261u ^ str[0])*16777619u;
        }
    };
    
    class TypeHash
    {
    public:
        template <unsigned int N>
        PB_INLINE TypeHash(const char (&str)[N])
        : _Hash(FnvHash<N, N>::Hash(str))
        {
        }
        
        operator const Uid&() const {
            return _Hash;
        }
        
    private:
        Uid _Hash;
    };
    
    Uid RuntimeTypeHash(std::string string);
}
