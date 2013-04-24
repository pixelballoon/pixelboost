#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "pixelboost/db/database.h"
#include "pixelboost/db/entity.h"
#include "pixelboost/db/record.h"
#include "pixelboost/db/register.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pb;

PB_DB_DECLARE_NAMESPACE(pixelboost)

Database* Database::_Instance = 0;

Database::Database()
{
    _ResolvingReferences = false;
    _DatabaseRoot = "";
    _State = luaL_newstate();
    luaL_openlibs(_State);
    
    PB_DB_REGISTER_NAMESPACE(this, pixelboost)
}

Database::~Database()
{
    lua_close(_State);
}

Database* Database::Instance()
{
    static Database* instance = new Database();
    return instance;
}

lua_State* Database::GetLuaState()
{
    return _State;
}

void Database::RegisterCreate(Uid type, CreateStruct createStruct)
{
    _StructCreate[type] = createStruct;
}

void Database::RegisterDestroy(Uid type, DestroyStruct destroyStruct)
{
    _StructDestroy[type] = destroyStruct;
}

void Database::RegisterDeserialise(Uid type, DeserialiseStruct deserialiseStruct)
{
    _StructDeserialise[type] = deserialiseStruct;
}

void Database::SetLocation(const std::string& location)
{
    _DatabaseRoot = location;
}

bool Database::OpenDatabase()
{
    std::string filename = _DatabaseRoot + "main.lua";
    
    std::string contents;
    
    auto file = pb::FileSystem::Instance()->OpenFile(filename);
    
    if (!file)
    {
        PbLogError("pb.database", "Unable to open database index (%s)", filename.c_str());
        return false;
    }
    
    file->ReadAll(contents);
    
    if (luaL_loadstring(_State, contents.c_str()) || lua_pcall(_State, 0, 0, 0))
    {
        PbLogError("pb.database", "Can't open file: %s", lua_tostring(_State, -1));
        return false;
    }
    
    lua_getglobal(_State, "records");
    if (lua_istable(_State, -1))
    {
        lua_len(_State, -1);
        if (!lua_isnumber(_State, -1))
            return false;
        
        int n = lua_tonumber(_State, -1);
        lua_pop(_State, 1);
        for (int i=1; i<=n; i++)
        {
            DbRecordDescription record;
            
            lua_rawgeti(_State, -1, i);
            if (!lua_istable(_State, -1))
                continue;
            
            lua_getfield(_State, -1, "type");
            record.Type = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "uid");
            record.Id = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "name");
            record.Name = lua_tostring(_State, -1);
            lua_pop(_State, 1);
            
            _RecordDescriptions.push_back(record);
            lua_pop(_State, 1);
        }
    }
    
    return true;
}

DbRecord* Database::OpenRecord(Uid recordId)
{
    RecordMap::iterator recordIt = _Records.find(recordId);
    
#ifdef PIXELBOOST_DISABLE_DEBUG
    if (recordIt != _Records.end())
        return recordIt->second;
#endif

    char filename[1024];
    sprintf(filename, "%srecords/%X.lua", _DatabaseRoot.c_str(), recordId);
    
    auto file = pb::FileSystem::Instance()->OpenFile(filename);
    
    std::string contents;
    
    if (!file)
    {
        PbLogError("pb.database", "Can't open file (%s)", filename);
    }
    
    file->ReadAll(contents);
    
    if (luaL_loadstring(_State, contents.c_str()) || lua_pcall(_State, 0, 0, 0))
    {
        PbLogError("pb.database", "Can't parse file (%s): %s", filename, lua_tostring(_State, -1));
        return 0;
    }
    
    char recordName[64];
    sprintf(recordName, "r%X", recordId);
    lua_getglobal(_State, recordName);
    
    if (!lua_istable(_State, -1))
        return 0;
    
    lua_getfield(_State, -1, "type");
    
    if (!lua_isstring(_State, -1))
    {
        lua_pop(_State, -1);
        PbLogError("pb.database", "Unknown record type");
        return 0;
    }
    
    Uid recordType = lua_tonumber(_State, -1);
    lua_pop(_State, 1);
    
    lua_getfield(_State, -1, "properties");
    
    if (!lua_istable(_State, -1))
    {
        lua_pop(_State, 1);
        return 0;
    }
    
    DbStructData* recordData;
    
    DbRecord* record;
    
    if (recordIt != _Records.end())
    {
        record = recordIt->second;
        recordData = record->GetData();
        Deserialise(recordType, record, recordData);
    }
    else
    {
        recordData = Create(recordType);
		record = new DbRecord(recordId, recordType, recordData);
        Deserialise(recordType, record, recordData);
        _Records[recordId] = record;
    }
    
    lua_pop(_State, 1);
    
    DbRecord::EntityMap currentEntities = record->_Entities;
    
    lua_getfield(_State, -1, "entities");
    
    if (lua_istable(_State, -1))
    {
        lua_len(_State, -1);
        if (!lua_isnumber(_State, -1))
            return 0;
        
        int n = lua_tonumber(_State, -1);
        lua_pop(_State, 1);
        for (int i=1; i<=n; i++)
        {
            Uid uid;
            Uid type;
            DbStructData* entityData;
            
            lua_rawgeti(_State, -1, i);
            if (!lua_istable(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            
            lua_getfield(_State, -1, "uid");
            
            if (!lua_isnumber(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            uid = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "type");
            if (!lua_isnumber(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            type = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            currentEntities.erase(uid);
            
            DbRecord::EntityMap::iterator entityIt = record->_Entities.find(uid);
            
            DbEntity* entity;
            
            if (entityIt != record->_Entities.end())
            {
                entity = entityIt->second;
                
                lua_getfield(_State, -1, "properties");
                
                if (!lua_istable(_State, -1))
                {
                    lua_pop(_State, 1);
                    continue;
                }
                
                entityData = entity->GetData();
                Deserialise(type, record, entityData);
                
                lua_pop(_State, 1);
                
                entity->Load();
                
                entity->structReloaded();
            } else {
                lua_getfield(_State, -1, "properties");
                
                if (!lua_istable(_State, -1))
                {
                    lua_pop(_State, 1);
                    continue;
                }
                
                entityData = Create(type);
                Deserialise(type, record, entityData);
                
                lua_pop(_State, 1);
                
                if (entityData)
                {
                    entity = new DbEntity(uid, type, entityData);
                    entity->Load();
                    record->AddEntity(entity);
                } else {
                    PbLogError("pb.database", "Unable to create entity of type (%u)", type);
                }
            }
            
            lua_pop(_State, 1);
        }
    }
    
    for (DbRecord::EntityMap::iterator it = currentEntities.begin(); it != currentEntities.end(); ++it)
    {
        record->RemoveEntity(it->second->GetUid());
        delete it->second;
    }
    
    lua_pop(_State, 1);
	
	record->ResolvePointers();
	ResolveReferences();
    
    return record;
}

bool Database::CloseRecord(Uid recordId)
{
    RecordMap::iterator it = _Records.find(recordId);
    
    if (it == _Records.end())
        return false;
    
    DbRecord* record = it->second;
    
    _Records.erase(it);
    
    delete record;
    
    return true;
}

DbStructData* Database::Create(Uid type)
{
    StructCreateMap::iterator it = _StructCreate.find(type);
    
    if (it == _StructCreate.end())
        return 0;
    
    return it->second();
}

void Database::Destroy(Uid type, DbStructData* structure)
{
    StructDestroyMap::iterator it = _StructDestroy.find(type);
    
    if (it != _StructDestroy.end())
        it->second(structure);
}

void Database::Deserialise(Uid type, DbRecord* record, DbStructData* data)
{
    StructDeserialiseMap::iterator it = _StructDeserialise.find(type);
    
    if (it == _StructDeserialise.end())
        return;
    
    it->second(this, record, data);
}

void Database::AddReference(Uid uid, DbReferenceBase* reference)
{
	RecordReference ref;
	ref.uid = uid;
	ref.reference = reference;
	
	_References.push_back(ref);
}

void Database::ResolveReferences()
{
    if (_ResolvingReferences)
        return;
    
    _ResolvingReferences = true;
    
	for (ReferenceList::iterator it = _References.begin(); it != _References.end();)
	{
		RecordMap::iterator record = _Records.find(it->uid);
		
		if (record != _Records.end())
		{
			it->reference->_Record = record->second;
			it = _References.erase(it);
		} else {
			DbRecord* record = OpenRecord(it->uid);
			if (record)
			{
				it = _References.begin();
			} else {
				it = _References.erase(it);
			}
		}
	}
	
	_References.clear();
    
    _ResolvingReferences = false;
}

const Database::RecordDescriptionList& Database::GetRecordDescriptions() const
{
    return _RecordDescriptions;
}

const Database::RecordMap& Database::GetRecords() const
{
    return _Records;
}

const DbRecord* Database::GetRecord(Uid uid) const
{
    RecordMap::const_iterator it = _Records.find(uid);
    
    if (it == _Records.end())
        return 0;
    
    return it->second;
}
