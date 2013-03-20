#pragma once

#include <string>

class Schema;

class SchemaParser
{
public:
    static bool Parse(Schema* schema, const std::string& input);
};
