#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace pb
{

    class StringHelpers
    {
    public:
        static std::vector<std::string>& SplitString(const std::string& s, char delim, std::vector<std::string>& items)
        {
            std::stringstream stream(s);
            std::string item;
            while(std::getline(stream, item, delim)) {
                items.push_back(item);
            }
            return items;
        }
        
        static inline std::string& TrimLeft(std::string& s)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
        }
        
        static inline std::string& TrimRight(std::string& s)
        {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
        }
        
        static inline std::string& Trim(std::string& s)
        {
            return TrimLeft(TrimRight(s));
        }
    };
    
}
