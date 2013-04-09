#pragma once

#include "lua.h"

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

#define PB_DB_DECLARE_STRUCT(type) struct type ## Definition; void PbRegisterDbStruct ## type  ## Definition (pb::Database* database); void PbDeserialise ## type  ## Definition (pb::Database* database, pb::DbRecord* record, void* data);

#define PB_DB_HAS_BINDING(type) friend void PB_Deserialise ## type ## Definition (void* data)

#define PB_DB_BEGIN_NAMESPACE(namespace) void PbRegisterDb_ ## namespace (pb::Database* database) {
#define PB_DB_REGISTER_STRUCT(type) PbRegisterDbStruct ## type ## Definition (database);
#define PB_DB_END_NAMESPACE }

#define PB_DB_DECLARE_NAMESPACE(namespace) void PbRegisterDb_ ## namespace(pb::Database* database);
#define PB_DB_REGISTER_NAMESPACE(database, namespace) PbRegisterDb_ ## namespace(database);

#define PB_DB_BEGIN_STRUCT(type) \
	namespace pb { \
		class Database; \
		class DbRecord; \
	} \
    void* PbCreate ## type ## Definition (); \
    void PbDestroy ## type ## Definition (void* object); \
    void PbRegister ## type ## Definition (); \
    void PbDeserialise ## type ## Definition (pb::Database* database, pb::DbRecord* record, void* data); \
    void PbRegisterDbStruct ## type ## Definition (pb::Database* database) { \
        database->RegisterCreate(pb::TypeHash(#type), &PbCreate ## type ## Definition ); \
        database->RegisterDestroy(pb::TypeHash(#type), &PbDestroy ## type ## Definition ); \
        database->RegisterDeserialise(pb::TypeHash(#type), &PbDeserialise ## type ## Definition ); \
    } \
    void* PbCreate ## type ## Definition () { \
        return new type ## Definition(); \
    } \
    void PbDestroy ## type ## Definition (void* object) { \
        delete (type ## Definition*)(object); \
    } \
    void PbDeserialise ## type ## Definition (pb::Database* database, pb::DbRecord* record, void* data) { \
        type ## Definition& object = *static_cast<type ## Definition*>(data); \
        (void)object; \
        lua_State* state = database->GetLuaState(); \
        (void)state;

#define PB_DB_DERIVED_STRUCT(type) { \
	PbDeserialise ## type ## Definition (database, record, data); \
}

#define __PB_DB_PARSE_BOOL(field) { \
    field = 0; \
    if (lua_isboolean(state, -1)) { \
        field = lua_toboolean(state, -1); \
    } \
}

#define __PB_DB_PARSE_INT(field) { \
    field = 0; \
    if (lua_isnumber(state, -1)) { \
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

#define PB_DB_FIELD_BOOL(field) { \
    lua_getfield(state, -1, #field); \
    __PB_DB_PARSE_BOOL(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_INT(field) { \
    lua_getfield(state, -1, #field); \
    __PB_DB_PARSE_INT(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_FLOAT(field) { \
    lua_getfield(state, -1, #field); \
    __PB_DB_PARSE_FLOAT(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRING(field) { \
    lua_getfield(state, -1, #field); \
    __PB_DB_PARSE_STRING(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_STRUCT(field, type) { \
    lua_getfield(state, -1, #field); \
    __PB_DB_PARSE_STRUCT(object.field, type ## Definition) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_POINTER(field) { \
    lua_getfield(state, -1, #field); \
	__PB_DB_PARSE_POINTER(object.field) \
    lua_pop(state, 1); \
}

#define PB_DB_FIELD_REFERENCE(field) { \
	lua_getfield(state, -1, #field); \
	__PB_DB_PARSE_REFERENCE(object.field) \
	lua_pop(state, 1); \
}

#define PB_DB_FIELD_ARRAY_BOOL(field) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, int) \
    __PB_DB_PARSE_BOOL(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_INT(field) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, int) \
    __PB_DB_PARSE_INT(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_FLOAT(field, name) { \
    __PB_DB_PARSE_ARRAY_START(object.field, name, float) \
    __PB_DB_PARSE_FLOAT(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_STRING(field) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, std::string) \
    __PB_DB_PARSE_STRING(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_STRUCT(field, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, type ## Definition) \
    __PB_DB_PARSE_STRUCT(arrayItem, type ## Definition), \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_POINTER(field, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, pb::DbPointer<type ## Definition>) \
    __PB_DB_PARSE_POINTER(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
}

#define PB_DB_FIELD_ARRAY_REFERENCE(field, type) { \
    __PB_DB_PARSE_ARRAY_START(object.field, #field, pb::DbReference<type ## Definition>) \
    __PB_DB_PARSE_REFERENCE(arrayItem) \
    __PB_DB_PARSE_ARRAY_END(object.field) \
} 

#define PB_DB_END_STRUCT }
