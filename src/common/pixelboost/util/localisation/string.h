#pragma once

#include <string>

namespace pb
{
    void StringToUnicode(std::wstring& dest, const std::string& src);
    void UnicodeToString(std::string& dest, const std::wstring& src);
    
    std::wstring StringToUnicode(const std::string& src);
    std::string UnicodeToString(const std::wstring& src);
}
