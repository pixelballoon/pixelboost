#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

struct lua_State;

namespace pixelboost
{
    
class Record;
    
struct RecordDescription
{
    std::string Name;
    Uid Type;
    Uid Uid;
};
    
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    
    static DatabaseManager* Instance();
    
public:
    lua_State* GetLuaState();
    
public:
    typedef void*(*CreateStruct)();
    
    void RegisterStruct(Uid type, CreateStruct createStruct);
    
    void OpenDatabase(const std::string& location);
    
    void OpenRecord(Uid recordId);
    
    void* Create(Uid type);
    
public:
    typedef std::vector<RecordDescription> RecordDescriptionList;
    typedef std::map<Uid, Record*> RecordMap;
    
    const RecordDescriptionList& GetRecordDescriptions() const;
    const RecordMap& GetRecords() const;
    const Record* GetRecord(Uid uid) const;
    
private:
    typedef std::map<Uid, CreateStruct> StructCreateMap;
    
    std::string _DatabaseRoot;
    
    RecordDescriptionList _RecordDescriptions;
    RecordMap _Records;
    
    StructCreateMap _StructCreate;
    
    static DatabaseManager* _Instance;
    
    lua_State* _State;
};
    
}
