#pragma once

#include <string>

#include "pixelboost/db/register.h"

struct ScriptDefinition
{
    std::string Filename;
};

struct ComponentDefinition
{
    
};

struct ScriptComponentDefinition : public ComponentDefinition
{
    pb::DbReference<ScriptDefinition> Script;
    std::string InlineScript;
};
