#pragma once

#ifdef _MSC_VER
#define PB_INLINE __forceinline
#else
#define PB_INLINE inline
#endif

namespace pb
{
    typedef unsigned int Uid;
    
    namespace
    {
        constexpr Uid TypeHash(char c, const char* input, Uid value)
        {
            return c == 0 ? value : TypeHash(*input, input+1, (value ^ c) * 16777619u);
        }
    }
    
    constexpr Uid TypeHash(const char* input)
    {
        return TypeHash(*input, input+1, 2166136261u);
    };
}
