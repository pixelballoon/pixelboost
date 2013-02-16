#pragma once

#include "lua.hpp"

#include "pixelboost/db/database.h"
#include "pixelboost/db/entity.h"
#include "pixelboost/db/record.h"

namespace pb
{
    class DbEntity;
    
    struct DbPointerBase
    {
    public:
        DbPointerBase(pb::DbEntity* entity = 0)
        {
            _Entity = entity;
        }
        
        pb::DbEntity* GetEntity() const
        {
            return _Entity;
        }
        
        operator bool() const
        {
            return _Entity;
        }
               
    protected:
        pb::DbEntity* _Entity;
        
        friend class DbRecord;
    };
    
    template <class T> struct DbPointer : public DbPointerBase
    {
        DbPointer(pb::DbEntity* entity = 0)
            : DbPointerBase(entity)
        {
            
        }
        
        const T* GetData() const
        {
            return _Entity->GetData<T>();
        }
        
        const T* operator->() const
        {
            return GetData();
        }
    };
    
    struct DbReferenceBase
    {
        DbReferenceBase(pb::DbRecord* record = 0)
        {
            _Record = record;
        }
        
        pb::DbRecord* GetRecord() const
        {
            return _Record;
        }
        
        operator bool() const
        {
            return _Record;
        }
        
    protected:
        pb::DbRecord* _Record;
        
        friend class Database;
    };
    
    template <class T> struct DbReference : public DbReferenceBase
    {
        DbReference(pb::DbRecord* record = 0)
            : DbReferenceBase(record)
        {
            
        }
        
        const T* GetData() const
        {
            return _Record->GetData<T>();
        }
        
        const T* operator->() const
        {
            return GetData();
        }
    };
}

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

#define __PB_DB_PARSE_INT(field) { \
    field = 0; \
    if (lua_isnumber(state, -1)) \
        field = lua_tonumber(state, -1); \
    } \
}

#define __PB_DB_PARSE_FLOAT(field) { \
    field = 0; \
    if (lua_isnumber(state, -1)) { \
        field = lua_tonumber(state, -1); \
    } \
}

#define __PB_DB_PARSE_STRING(field) { \
    if (lua_isstring(state, -1)) { \
        field = lua_tostring(state, -1); \
    } \
}

#define __PB_DB_PARSE_STRUCT(field, type) { \
    if (lua_istable(state, -1)) { \
        PbDeserialise ## type (database, record, &field); \
    } \
}

#define __PB_DB_PARSE_POINTER(field) { \
    field = 0; \
    if (lua_isnumber(state, -1)) { \
        record->AddPointer(lua_tonumber(state, -1), &field); \
    } \
}

#define __PB_DB_PARSE_REFERENCE(field) { \
    field = 0; \
    if (lua_isnumber(state, -1)) { \
        database->AddReference(lua_tonumber(state, -1), &field); \
    } \
}

#define __PB_DB_PARSE_ARRAY_START(field, name, type) { \
    field.clear(); \
    lua_getfield(state, -1, name); \
    if (lua_istable(state, -1)) {\
        lua_len(state, -1); \
        if (lua_isnumber(state, -1)) { \
            int numArrayItems = lua_tonumber(state, -1); \
            lua_pop(state, 1); \
            field.reserve(numArrayItems); \
            for (int i=1; i<=numArrayItems; i++) { \
                lua_rawgeti(state, -1, i); \
                type tempArrayItem; \
                field.push_back(tempArrayItem); \
                type& arrayItem = field.back(); \

#define __PB_DB_PARSE_ARRAY_END(field) \
                lua_pop(state, 1); \
            } \
        } \
    } \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_INT(field, name) { \
    lua_getfield(state, -1, name); \
    __PB_DB_PARSE_INT(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_FLOAT(field, name) { \
    lua_getfield(state, -1, name); \
    __PB_DB_PARSE_FLOAT(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRING(field, name) { \
    lua_getfield(state, -1, name); \
    __PB_DB_PARSE_STRING(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRUCT(field, name, type) { \
    lua_getfield(state, -1, name); \
    __PB_DB_PARSE_STRUCT(object.field, type) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_POINTER(field, name) { \
    lua_getfield(state, -1, name); \
	__PB_DB_PARSE_POINTER(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_REFERENCE(field, name) { \
	lua_getfield(state, -1, name); \
	__PB_DB_PARSE_REFERENCE(object.field) \
	lua_pop(state, 1); \
}

#define PB_DB_FIELD_ARRAY_INT(field, name) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, int) \
    __PB_DB_PARSE_INT(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_FLOAT(field, name) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, float) \
    __PB_DB_PARSE_FLOAT(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_STRING(field, name) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, std::string) \
    __PB_DB_PARSE_STRING(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_STRUCT(field, name, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, type) \
    __PB_DB_PARSE_STRUCT(arrayItem, type), \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_POINTER(field, name, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, pb::DbPointer<type>) \
    __PB_DB_PARSE_POINTER(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_REFERENCE(field, name, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, pb::DbReference<type>) \
    __PB_DB_PARSE_REFERENCE(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
} 

#define PB_DB_END_STRUCT }
