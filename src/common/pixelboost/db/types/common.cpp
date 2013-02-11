#include "pixelboost/db/types/common.h"
#include "pixelboost/db/register.h"

PB_DB_BEGIN_STRUCT(Vector2Definition, "Vector2")
    PB_DB_FIELD_FLOAT(X, "X")
    PB_DB_FIELD_FLOAT(Y, "Y")
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(Vector3Definition, "Vector3")
    PB_DB_FIELD_FLOAT(X, "X")
    PB_DB_FIELD_FLOAT(Y, "Y")
    PB_DB_FIELD_FLOAT(Z, "Z")
PB_DB_END_STRUCT

PB_DB_BEGIN_STRUCT(ColorDefinition, "Color")
    PB_DB_FIELD_FLOAT(R, "R")
    PB_DB_FIELD_FLOAT(G, "G")
    PB_DB_FIELD_FLOAT(B, "B")
    PB_DB_FIELD_FLOAT(A, "A")
PB_DB_END_STRUCT
