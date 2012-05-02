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
    typedef void*(*CreateStruct)();
    
    void RegisterStruct(const std::string& name, CreateStruct createStruct);
    
    void OpenDatabase(const std::string& location);
    
    void OpenRecord(Uid recordId);
    
    void* Create(const std::string& name);
    
public:
    typedef std::vector<Record*> RecordList;
    
    const RecordList& GetRecords() const;
    const Record* GetRecord(Uid uid);
    
private:
    typedef std::map<std::string, CreateStruct> StructCreateMap;
    typedef std::map<Uid, Record*> RecordMap;
    typedef std::vector<Uid> RecordIdList;
    
    RecordIdList _RecordIds;
    RecordList _Records;
    RecordMap _UidMap;
    
    StructCreateMap _StructCreate;
    
    static DatabaseManager* _Instance;
    
    lua_State* _State;
};
    
}
