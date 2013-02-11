#include "pixelboost/db/types/components.h"
#include "pixelboost/db/register.h"

PB_DB_DECLARE_STRUCT(ComponentDefinition)

PB_DB_BEGIN_STRUCT(ScriptDefinition, "Script")
    PB_DB_FIELD_STRING(Filename, "Filename")
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(ComponentDefinition, "Component")
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(ScriptComponentDefinition, "ScriptComponent")
    PB_DB_DERIVED_STRUCT(ComponentDefinition)
    PB_DB_FIELD_REFERENCE(Script, "Script")
    PB_DB_FIELD_STRING(InlineScript, "InlineScript")
PB_DB_END_STRUCT
