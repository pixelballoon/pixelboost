#include <fstream>
#include <dirent.h>
#include <errno.h>

#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"

#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"

using namespace pixeleditor;

bool GetDirectoryListing(const std::string& directory, std::vector<std::string>& files)
{
    DIR* dir;
    dirent* entry;
    dir = opendir(directory.c_str());
    
    if (!dir) // TODO: Log to error, use errno for error number
        return false;
    
    while ((entry = readdir(dir)))
    {
        if (entry->d_type == DT_REG)
            files.push_back(std::string(entry->d_name));
    }
    
    closedir(dir);
    
    return true;
}

Project::Project()
    : _IsOpen(false)
    , _Location("")
    , _Schema(0)
{
    _Name = "Project";
    _Schema = new Schema();
}

Project::~Project()
{
    Close();
}

bool Project::Open(const std::string& directory)
{
    if (_IsOpen)
        Close();
    
    _Location = directory;
    
    if (_Location.length() == 0)
        return false;
    
    if (_Location[_Location.length()-1] != '/')
        _Location += "/";
    
    OpenConfig(_Location + "project.prj");
    
    if (!_Schema->Open(_Location + "schema/"))
        return false;
      
    _IsOpen = true;
    
    projectOpened(this);
    
    std::vector<std::string> files;
    GetDirectoryListing(_Location + "records/", files);
    
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        if (it->length() > 0 && it->at(0) == '.')
            continue;
        
        if (it->length() < 4 || it->substr(it->length()-4, 4) != ".txt")
            continue;
        
        LoadRecord(_Location + "records/" + *it);
    }
    
    return true;
}
    
bool Project::OpenConfig(const std::string& filename)
{
    std::string file = pb::FileHelpers::FileToString(filename.c_str());
    
    json::Object config;
    
    json::Reader::Read(config, file);
    
    // Image root
    json::Array& imageRoots = config["image_roots"];
    for (json::Array::iterator it = imageRoots.Begin(); it != imageRoots.End(); ++it)
    {
        json::String imageRoot = *it;
        _Config.imageRoots.push_back(_Location + imageRoot.Value());
    }
    
    // Export directory
    json::String& exportDir = config["export_dir"];
    _Config.exportDir = _Location + exportDir.Value();
    
    // Pixel unit (How many pixels in 1m)
    json::Number& pixelUnit = config["pixel_unit"];
    _Config.pixelUnit = pixelUnit.Value();
    
    return true;
}

bool Project::Close()
{
    if (!_IsOpen)
        return false;
    
    _Location = "";
    
    for (RecordMap::iterator it = _Records.begin(); it != _Records.end(); ++it)
    {
        it->second->Close();
        recordRemoved(this, it->second);
        delete it->second;
    }
    _Records.clear();
    
    _Schema->Close();
    _Uids.clear();
    
    _IsOpen = false;
    
    projectClosed(this);
    
    return true;
}

bool Project::Save()
{
    for (RecordMap::iterator it = _Records.begin(); it != _Records.end(); ++it)
        it->second->Save();
    
    projectSaved(this);
    
    return true;
}

bool Project::Export()
{
    // TODO: Make OS independant
    char cmd[2048];
    std::string outputDir = GetConfig().exportDir;
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);

    std::string location = outputDir + "main.lua";
    
    std::fstream file(location.c_str(), std::fstream::out | std::fstream::trunc);
    
    file << "records = {" << std::endl;

    for (RecordMap::iterator it = _Records.begin(); it != _Records.end();)
    {
        it->second->ExportLua();
        
        char recordString[32];
        sprintf(recordString, "{ name = \"%s\", type = %u, uid = %u }", it->second->GetName().c_str(), it->second->GetTypeHash(), it->second->GetUid());
        
        file << recordString;
        
        ++it;
        
        if (it != _Records.end())
            file << ",";
        
        file << std::endl;
    }
    
    file << "}" << std::endl;
    
    file.close();
    
    projectExported(this);
    
    return true;
}

bool Project::IsOpen()
{
    return _IsOpen;
}
    
const std::string& Project::GetLocation() const
{
    return _Location;
}

const std::string& Project::GetName() const
{
    return _Name;
}
    
Entity* Project::GetEntity(Uid uid) const
{
    for (RecordMap::const_iterator it = _Records.begin(); it != _Records.end(); ++it)
    {
        Entity* entity = it->second->GetEntity(uid);
        if (entity)
            return entity;
    }
    
    return 0;
}
    
Record* Project::GetRecord(Uid uid) const
{
    RecordMap::const_iterator it = _Records.find(uid);
    
    if (it != _Records.end())
        return it->second;
    
    return 0;
}
    
Record* Project::GetRecordByName(const std::string& recordName) const
{
    for (RecordMap::const_iterator it = _Records.begin(); it != _Records.end(); ++it)
    {
        if (recordName == it->second->GetName())
            return it->second;
    }
    
    return 0;
}
    
Uid Project::CalculateUid(Uid min, Uid max)
{
    for (int i=0; i<128; i++)
    {
        unsigned long id = rand();
        UidSet::iterator it = _Uids.find(id);
        if (id > min && id < max && it == _Uids.end())
        {
            _Uids.insert(id);
            return id;
        }
    }
    
    // TODO : Assert here!
    
    return 0;
    
}

bool Project::RegisterUid(Uid uid)
{
    if (_Uids.find(uid) != _Uids.end())
        return false;
    
    _Uids.insert(uid);
    
    return true;
}

bool Project::ReleaseUid(Uid uid)
{
    UidSet::iterator it = _Uids.find(uid);
    
    if (it == _Uids.end())
        return false;
    
    _Uids.erase(it);
    
    return true;
}
    
const Project::ProjectConfig& Project::GetConfig() const
{
    return _Config;
}

const Project::RecordMap& Project::GetRecords() const
{
    return _Records;
}

bool Project::AddRecord(const std::string& name, const std::string& type)
{
    if (GetRecordByName(name))
        return false;
    
    const SchemaRecord* schemaRecord = GetSchema()->GetRecordByName(type);
    
    Record* record = new Record(this, schemaRecord, name);
    
    _Records[record->GetUid()] = record;
    
    recordAdded(this, record);
    
    return true;
}
    
bool Project::LoadRecord(const std::string& filename)
{
    std::cout << filename << std::endl;
    Record* record = new Record(this);
    
    record->Open(filename);
    
    std::string name = record->GetName();
    
    Record* existing = GetRecordByName(name);
    
    if (existing)
    {
        delete record;
        return false;
    }
    
    _Records[record->GetUid()] = record;
    
    recordAdded(this, record);
    
    return true;
}

bool Project::RemoveRecord(std::string const &name, bool erase)
{
    Record* record = GetRecordByName(name);
    
    if (!record)
        return false;

    RecordMap::iterator it = _Records.find(record->GetUid());
    
    _Records.erase(it);
    
    recordRemoved(this, record);
    
    if (erase)
        delete record;
    
    return true;
}
    
Schema* Project::GetSchema()
{
    return _Schema;
}
