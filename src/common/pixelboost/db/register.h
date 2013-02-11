#pragma once

#include "lua.hpp"

#include "pixelboost/db/database.h"

#define PB_DB_DECLARE_CLASS(type) class type ; void PbRegisterDb ## type ();
#define PB_DB_DECLARE_STRUCT(type) struct type ; void PbRegisterDbStruct ## type (pb::Database* database); void PbDeserialise ## type (pb::Database* database, pb::DbRecord* record, void* data);

#define PB_DB_HAS_BINDING(type) friend void PB_Deserialise ## type (void* data)

#define PB_DB_BEGIN_NAMESPACE(namespace) void PbRegisterDb_ ## namespace (pb::Database* database) {
#define PB_DB_REGISTER_STRUCT(type) PbRegisterDbStruct ## type (database);
#define PB_DB_END_NAMESPACE }

#define PB_DB_DECLARE_NAMESPACE(namespace) void PbRegisterDb_ ## namespace(pb::Database* database);
#define PB_DB_REGISTER_NAMESPACE(database, namespace) PbRegisterDb_ ## namespace(database);

#define PB_DB_BEGIN_STRUCT(type, name) \
	namespace pb { \
		class Database; \
		class DbRecord; \
	} \
    void* PbCreate ## type (); \
    void PbDestroy ## type (void* object); \
    void PbRegister ## type (); \
    void PbDeserialise ## type (pb::Database* database, pb::DbRecord* record, void* data); \
    void PbRegisterDbStruct ## type (pb::Database* database) { \
        database->RegisterCreate(pb::TypeHash(name), &PbCreate ## type ); \
        database->RegisterDestroy(pb::TypeHash(name), &PbDestroy ## type ); \
        database->RegisterDeserialise(pb::TypeHash(name), &PbDeserialise ## type ); \
    } \
    void* PbCreate ## type () { \
        return new type(); \
    } \
    void PbDestroy ## type (void* object) { \
        delete (type*)(object); \
    } \
    void PbDeserialise ## type (pb::Database* database, pb::DbRecord* record, void* data) { \
        type& object = *static_cast<type*>(data); \
        (void)object; \
        lua_State* state = database->GetLuaState(); \
        (void)state;

#define PB_DB_DERIVED_STRUCT(type) { \
	PbDeserialise ## type (database, record, data); \
}

#define PB_DB_FIELD_INT(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isnumber(state, -1)) { \
        object.field = lua_tonumber(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_FLOAT(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isnumber(state, -1)) { \
        object.field = lua_tonumber(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRING(field, name) { \
    lua_getfield(state, -1, name); \
    if (lua_isstring(state, -1)) { \
        object.field = lua_tostring(state, -1); \
    } \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRUCT(field, name, type) { \
    lua_getfield(state, -1, name); \
    if (lua_istable(state, -1)) { \
        PbDeserialise ## type (database, record, &object.field); \
    } \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_POINTER(field, name) { \
    lua_getfield(state, -1, name); \
	object.field = 0; \
	if (lua_isnumber(state, -1)) { \
		record->AddPointer(lua_tonumber(state, -1), (void**)&object.field); \
	} \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_REFERENCE(field, name) { \
	lua_getfield(state, -1, name); \
	object.field = 0; \
	if (lua_isnumber(state, -1)) { \
		database->AddReference(lua_tonumber(state, -1), (void**)&object.field); \
	} \
	lua_pop(state, 1); \
}

#define PB_DB_END_STRUCT }