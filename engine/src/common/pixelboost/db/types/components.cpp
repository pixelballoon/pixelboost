#include "pixelboost/db/types/components.h"
#include "pixelboost/db/register.h"

PB_DB_DECLARE_STRUCT(ComponentDefinition)

PB_DB_BEGIN_STRUCT(Script)
    PB_DB_FIELD_STRING(Filename)
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(Component)
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(ScriptComponent)
    PB_DB_DERIVED_STRUCT(Component)
    PB_DB_FIELD_REFERENCE(Script)
    PB_DB_FIELD_STRING(InlineScript)
PB_DB_END_STRUCT
