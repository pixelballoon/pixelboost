#include "pixelboost/db/register.h"

PB_DB_DECLARE_STRUCT(ScriptDefinition);

PB_DB_DECLARE_STRUCT(ComponentDefinition);
PB_DB_DECLARE_STRUCT(ScriptComponentDefinition);

PB_DB_BEGIN_NAMESPACE(pixelboost)
    PB_DB_REGISTER_STRUCT(ScriptDefinition)
    PB_DB_REGISTER_STRUCT(ComponentDefinition)
    PB_DB_REGISTER_STRUCT(ScriptComponentDefinition)
PB_DB_END_NAMESPACE
