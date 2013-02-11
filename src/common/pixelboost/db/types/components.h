#pragma once

#include <string>

struct ScriptDefinition
{
    std::string Filename;
};

struct ComponentDefinition
{
    
};

struct ScriptComponentDefinition : public ComponentDefinition
{
    ScriptDefinition* Script;
    std::string InlineScript;
};
