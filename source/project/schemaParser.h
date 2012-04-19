#pragma once

#include <string>

namespace pixeleditor
{
    class Schema;
    
    class SchemaParser
    {
    public:
        static bool Parse(Schema* schema, const std::string& input);
    };
}