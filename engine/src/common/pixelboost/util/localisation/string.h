#pragma once

#include <stdint.h>
#include <string>

namespace pb
{
    typedef std::basic_string<uint32_t> wstring;
    
    void UTF8toUTF32(wstring& dest, const std::string& src);
    void UTF32toUTF8(std::string& dest, const wstring& src);
    
    wstring UTF8toUTF32(const std::string& src);
    std::string UTF32toUTF8(const wstring& src);
}
