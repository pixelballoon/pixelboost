#include "libpixel/db/manager.h"
#include "libpixel/db/record.h"
#include "libpixel/file/fileHelpers.h"

namespace libpixeldb
{
    
DatabaseManager* DatabaseManager::_Instance = 0;

DatabaseManager::DatabaseManager()
{
    _Instance = this;
}

DatabaseManager::~DatabaseManager()
{
    _Instance = 0;
}

DatabaseManager* DatabaseManager::Instance()
{
    return _Instance;
}

void DatabaseManager::RegisterStruct(const std::string& name, CreateStruct createStruct)
{
    _StructCreate[name] = createStruct;
}
    
void DatabaseManager::OpenDatabase(const std::string& location)
{
    
}

void DatabaseManager::OpenRecord(const std::string& filename)
{
    std::string recordData = libpixel::FileHelpers::FileToString(filename);
        
    json::Object record;
    
    json::Reader::Read(record, recordData);

    json::String& type = record["Type"];
    
    Struct* s = DatabaseManager::Instance()->Create(type.Value());
    
    if (!s)
        return;
    
    if (s->GetType() != 2) // kDbRecord
    {
        delete s;
        return;
    }
    
    s->Deserialise(record, 0);
    
    _Records.push_back(static_cast<Record*>(s));
    _UidMap[s->_Uid] = static_cast<Record*>(s);
}
    
Struct* DatabaseManager::Create(const std::string& name)
{
    StructCreateMap::iterator it = _StructCreate.find(name);
    
    if (it == _StructCreate.end())
        return 0;
    
    return it->second();
}

}
