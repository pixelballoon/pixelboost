#pragma once

#include <map>
#include <vector>

#include "pixelboost/db/struct.h"

struct lua_State;

namespace pb
{
    
class Record;
    
struct RecordDescription
{
    std::string Name;
    Uid Type;
    Uid Uid;
};
    
class Database
{
public:
    Database();
    ~Database();
    
    static Database* Instance();
    
public:
    lua_State* GetLuaState();
    
public:
    typedef void*(*CreateStruct)();
    
    void RegisterStruct(Uid type, CreateStruct createStruct);
    
    void OpenDatabase(const std::string& location);
    
    Record* OpenRecord(Uid recordId);
    
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
    
    static Database* _Instance;
    
    lua_State* _State;
};
    
}
