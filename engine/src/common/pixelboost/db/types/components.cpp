#include "pixelboost/db/types/components.h"

PB_DB_BEGIN_STRUCT(Script)
    PB_DB_FIELD_STRING(Filename)
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(Component)
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT_DERIVED(ScriptComponent, Component)
    PB_DB_FIELD_REFERENCE(Script, Script)
    PB_DB_FIELD_STRING(InlineScript)
PB_DB_END_STRUCT
