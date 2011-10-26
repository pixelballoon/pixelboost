#pragma once

#include <map>
#include <vector>

#include "libpixel/db/struct.h"

namespace libpixeldb
{
    
class Record;
    
class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    
    static DatabaseManager* Instance();
    
public:
    typedef Struct*(*CreateStruct)();
    
    void RegisterStruct(const std::string& name, CreateStruct createStruct);
    
    void OpenDatabase(const std::string& location);
    
    void OpenRecord(const std::string& filename);
    
    Struct* Create(const std::string& name);
    
public:
    typedef std::vector<Record*> RecordList;
    
    const RecordList& GetRecords() const;
    const Record* GetRecord(Uid uid);
    
private:
    typedef std::map<std::string, CreateStruct> StructCreateMap;
    typedef std::map<Uid, Record*> RecordMap;
    
    RecordList _Records;
    RecordMap _UidMap;
    
    StructCreateMap _StructCreate;
    
    static DatabaseManager* _Instance;
};
    
}
