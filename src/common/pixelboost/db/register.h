#pragma once

#include "pixelboost/external/lua/lua.hpp"

#include "pixelboost/db/database.h"

#define PIXELBOOST_DECLARE_CLASS(type) class type ; void PB_Register ## type ();
#define PIXELBOOST_DECLARE_STRUCT(type) struct type ; void PB_Register ## type (); void PB_Deserialise ## type (void* data);

#define PIXELBOOST_HAS_BINDING(type) friend void PB_Deserialise ## type (void* data)

#define PIXELBOOST_START_REGISTER(name) void name () {
#define PIXELBOOST_REGISTER(name) PB_Register ## name ();
#define PIXELBOOST_END_REGISTER }

#define PIXELBOOST_START_STRUCT(type, name) \
    void* PB_Create ## type (); \
    void PB_Register ## type (); \
    void PB_Deserialise ## type (void* data); \
    void PB_Register ## type () { \
        pb::Database::Instance()->RegisterCreate(pb::TypeHash(name), &PB_Create ## type ); \
        pb::Database::Instance()->RegisterDeserialise(pb::TypeHash(name), &PB_Deserialise ## type ); \
    } \
    void* PB_Create ## type () { \
        type* structObject = new type(); \
        return structObject; \
    } \
    void PB_Deserialise ## type (void* data) { \
        type& object = *static_cast<type*>(data); \
        (void)object; \
        lua_State* state = pb::Database::Instance()->GetLuaState(); \
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
        PB_Deserialise ## type (&object.field); \
    } \
    lua_pop(state, 1); \
}

#define PIXELBOOST_END_STRUCT }
