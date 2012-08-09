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
    };
    
}
