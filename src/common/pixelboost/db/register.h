#pragma once

#include "pixelboost/external/lua/lua.hpp"

#include "pixelboost/db/manager.h"

#define PIXELBOOST_DECLARE_CLASS(type) class type ; void PB_Register ## type ();
#define PIXELBOOST_DECLARE_STRUCT(type) struct type ; void PB_Register ## type ();

#define PIXELBOOST_HAS_BINDING(type) friend void PB_Deserialise ## type (class pixelboost::Record* record, type& object)

#define PIXELBOOST_START_REGISTER(name) void name () {
#define PIXELBOOST_REGISTER(name) PB_Register ## name ();
#define PIXELBOOST_END_REGISTER }

#define PIXELBOOST_START_STRUCT(type, name) \
    class pixelboost::Record; \
    void* PB_Create ## type (pixelboost::Record* record); \
    void PB_Register ## type (); \
    void PB_Deserialise ## type (pixelboost::Record* record, type& object); \
    void PB_Register ## type () { \
        pixelboost::DatabaseManager::Instance()->RegisterStruct(pixelboost::TypeHash(name), &PB_Create ## type ); \
    } \
    void* PB_Create ## type (pixelboost::Record* record) { \
        type* structObject = new type(); \
        PB_Deserialise ## type (record, *structObject); \
        return structObject; \
    } \
    void PB_Deserialise ## type (pixelboost::Record* record, type& object) { \
        lua_State* state = pixelboost::DatabaseManager::Instance()->GetLuaState(); \
        (void)state;

#define PIXELBOOST_FIELD_INT(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isnumber(state, -1)) { \
        object.field = lua_tonumber(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PIXELBOOST_FIELD_FLOAT(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isnumber(state, -1)) { \
        object.field = lua_tonumber(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PIXELBOOST_FIELD_STRING(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isstring(state, -1)) { \
        object.field = lua_tostring(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PIXELBOOST_FIELD_STRUCT(field, name, type) { \
    lua_getfield(state, -1, name); \
    if (lua_istable(state, -1)) { \
        PB_Deserialise ## type (record, object.field); \
    } \
    lua_pop(state, 1); \
}

#define PIXELBOOST_END_STRUCT }
