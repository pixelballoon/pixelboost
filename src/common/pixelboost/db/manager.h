#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

struct lua_State;

namespace pixelboost
{
    
class Record;
    
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    
    static DatabaseManager* Instance();
    
public:
    lua_State* GetLuaState();
    
public:
    typedef void*(*CreateStruct)(Record* record);
    
    void RegisterStruct(Uid type, CreateStruct createStruct);
    
    void OpenDatabase(const std::string& location);
    
    void OpenRecord(Uid recordId);
    
    void* Create(Record* record, Uid type);
    
public:
    typedef std::vector<Uid> RecordIdList;
    typedef std::map<Uid, Record*> RecordMap;
    
    const RecordIdList& GetRecordIds() const;
    const RecordMap& GetRecords() const;
    const Record* GetRecord(Uid uid) const;
    
private:
    typedef std::map<Uid, CreateStruct> StructCreateMap;
    
    std::string _DatabaseRoot;
    
    RecordIdList _RecordIds;
    RecordMap _Records;
    
    StructCreateMap _StructCreate;
    
    static DatabaseManager* _Instance;
    
    lua_State* _State;
};
    
}
