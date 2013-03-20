#include "pixelboost/db/register.h"

PB_DB_DECLARE_STRUCT(Script);

PB_DB_DECLARE_STRUCT(Component);
PB_DB_DECLARE_STRUCT(ScriptComponent);

PB_DB_BEGIN_NAMESPACE(pixelboost)
    PB_DB_REGISTER_STRUCT(Script)
    PB_DB_REGISTER_STRUCT(Component)
    PB_DB_REGISTER_STRUCT(ScriptComponent)
PB_DB_END_NAMESPACE
