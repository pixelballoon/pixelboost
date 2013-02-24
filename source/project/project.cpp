#include <fstream>
#include <dirent.h>
#include <errno.h>

#include "json/reader.h"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"

#include "core.h"

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
    
    pb::FileSystem::Instance()->OverrideWriteDirectory(directory);
    pb::FileSystem::Instance()->MountReadLocation(directory, "editor_project", true);
    
    _Location = directory;
    
    if (_Location.length() == 0)
        return false;
    
    if (_Location[_Location.length()-1] != '/')
        _Location += "/";
    
    OpenConfig("editor_project/project.prj");
    
    if (!_Schema->Open(_Config.commonSchema, "editor_project/schema/main.txt"))
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
        
        LoadRecord("editor_project/records/" + *it);
    }
    
    return true;
}
    
bool Project::OpenConfig(const std::string& filename)
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
    
    std::string contents;
    
    if (file)
    {
        file->ReadAll(contents);
        delete file;
    }
    
    json::Object config;
    
    json::Reader::Read(config, contents);
    
    json::String& projectRoot = config["project_root"];
    _Config.projectRoot = projectRoot;
    
    json::String& commonSchema = config["common_schema"];
    _Config.commonSchema = commonSchema;
    
    // Image roots
    json::Array& imageRoots = config["image_roots"];
    for (json::Array::iterator it = imageRoots.Begin(); it != imageRoots.End(); ++it)
    {
        json::String imageRoot = *it;
        _Config.imageRoots.push_back(_Location + imageRoot.Value());
    }
    
    // Model roots
    json::Array& modelRoots = config["model_roots"];
    for (json::Array::iterator it = modelRoots.Begin(); it != modelRoots.End(); ++it)
    {
        json::String modelRoot = *it;
        _Config.modelRoots.push_back(_Location + modelRoot.Value());
    }
    
    // Export directory
    json::String& exportDir = config["export_dir"];
    _Config.exportDir = _Location + exportDir.Value();
    
    // Pixel unit (How many pixels in 1m)
    json::Number& pixelUnit = config["pixel_unit"];
    _Config.pixelUnit = pixelUnit.Value();
    
    json::Object& assets = config["assets"];
    _Config.assets = assets;
    
    for (auto it = _Config.imageRoots.begin(); it != _Config.imageRoots.end(); ++it)
    {
        pb::FileSystem::Instance()->MountReadLocation(_Location + *it, "editor_images", true);
    }
    for (auto it = _Config.modelRoots.begin(); it != _Config.modelRoots.end(); ++it)
    {
        pb::FileSystem::Instance()->MountReadLocation(_Location + *it, "editor_models", true);
    }
    
    pb::FileSystem::Instance()->MountReadLocation(_Location + _Config.projectRoot, "/", true);
    
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

bool Project::Export(bool networkExport)
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
        
        char recordString[1024];
        snprintf(recordString, 1024, "{ name = \"%s\", type = %u, uid = %u }", it->second->GetName().c_str(), it->second->GetTypeHash(), it->second->GetUid());
        
        file << recordString;
        
        ++it;
        
        if (it != _Records.end())
            file << ",";
        
        file << std::endl;
    }
    
    file << "}" << std::endl;
    
    file.close();
    
    projectExported(this);
    
    if (networkExport)
    {
        if (!Core::Instance()->GetNetworkManager()->GetClientConnection().IsOpen())
            return false;
        
        pb::File* file;
        
        pb::NetworkMessage message;
        message.SetProtocol('DBDG');
        message.WriteString("main.lua");
        file = pb::FileSystem::Instance()->OpenFile(outputDir+"main.lua");
        std::string main;
        if (file)
        {
            file->ReadAll(main);
            delete file;
            file = 0;
        }
        message.WriteString(main.c_str());
        Core::Instance()->GetNetworkManager()->GetClientConnection().Send(message);
        
        for (RecordMap::iterator it = _Records.begin(); it != _Records.end(); ++it)
        {
            char recordName[1024];
            sprintf(recordName, "records/%X.lua", it->second->GetUid());
            
            pb::NetworkMessage message;
            message.SetProtocol('DBDG');
            message.WriteString(recordName);
            file = pb::FileSystem::Instance()->OpenFile(outputDir+recordName);
            std::string main;
            if (file)
            {
                file->ReadAll(main);
                delete file;
                file = 0;
            }
            message.WriteString(main.c_str());
            Core::Instance()->GetNetworkManager()->GetClientConnection().Send(message);
        }
    }
    
    return true;
}

bool Project::IsOpen()
{
    return _IsOpen;
}
    
const std::string& Project::GetName() const
{
    return _Name;
}
    
ProjectEntity* Project::GetEntity(Uid uid) const
{
    for (RecordMap::const_iterator it = _Records.begin(); it != _Records.end(); ++it)
    {
        ProjectEntity* entity = it->second->GetEntity(uid);
        if (entity)
            return entity;
    }
    
    return 0;
}
    
ProjectRecord* Project::GetRecord(Uid uid) const
{
    RecordMap::const_iterator it = _Records.find(uid);
    
    if (it != _Records.end())
        return it->second;
    
    return 0;
}
    
ProjectRecord* Project::GetRecordByName(const std::string& recordName) const
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
        unsigned long id = (rand() % max-min)+min;
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
    
    ProjectRecord* record = new ProjectRecord(this, schemaRecord, name);
    
    _Records[record->GetUid()] = record;
    
    recordAdded(this, record);
    
    return true;
}
    
bool Project::LoadRecord(const std::string& filename)
{
    ProjectRecord* record = new ProjectRecord(this);
    
    record->Open(filename);
    
    std::string name = record->GetName();
    
    ProjectRecord* existing = GetRecordByName(name);
    
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
    ProjectRecord* record = GetRecordByName(name);
    
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
