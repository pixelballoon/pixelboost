#pragma once

#include "pixelboost/db/manager.h"

#define PIXELBOOST_DECLARE_CLASS(type) class type ; void PB_Register ## type ();
#define PIXELBOOST_DECLARE_STRUCT(type) struct type ; void PB_Register ## type ();

#define PIXELBOOST_START_REGISTER(name) void name () {
#define PIXELBOOST_REGISTER(name) PB_Register ## name ();
#define PIXELBOOST_END_REGISTER }

#define PIXELBOOST_START_STRUCT(name, type) void* PB_Create ## type (); void PB_Register ## type (); void PB_Register ## type () { pixelboost::DatabaseManager::Instance()->RegisterStruct(name, &PB_Create ## type ); } void* PB_Create ## type () { type* structObject = new type();

#define PIXELBOOST_FIELD_INT(name) {}

#define PIXELBOOST_END_STRUCT return structObject; }
