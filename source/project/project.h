#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "sigslot/signal.h"

#include "json/reader.h"

#include "project/definitions.h"

namespace pixeleditor
{

class ProjectEntity;
class ProjectRecord;
class Schema;

class Project
{
public:
    Project();
    ~Project();
    
    bool Open(const std::string& directory);
    bool Close();
    bool Save();
    bool Export(bool networkExport);
    
    bool IsOpen();
    
    const std::string& GetName() const;
    
    ProjectEntity* GetEntity(Uid uid) const;
    ProjectRecord* GetRecord(Uid uid) const;
    ProjectRecord* GetRecordByName(const std::string& recordName) const;
    
    Uid CalculateUid(Uid min=1, Uid max=0xffffffUL);
    bool RegisterUid(Uid uid);
    bool ReleaseUid(Uid uid);

public:
    struct ProjectConfig
    {
        std::string projectRoot;
        std::string commonSchema;
        std::vector<std::string> imageRoots;
        std::vector<std::string> modelRoots;
        std::string exportDir;
        unsigned int pixelUnit;
        json::Object assets;
    };
    
    const ProjectConfig& GetConfig() const;
    
    Schema* GetSchema();
    
public:
    typedef std::map<Uid, ProjectRecord*> RecordMap;
    
    const RecordMap& GetRecords() const;
    bool AddRecord(const std::string& name, const std::string& type);
    bool LoadRecord(const std::string& filename);
    bool RemoveRecord(const std::string& name, bool erase=true);    
    
public:
    sigslot::Signal1<Project*> projectOpened;
    sigslot::Signal1<Project*> projectClosed;
    sigslot::Signal1<Project*> projectSaved;
    sigslot::Signal1<Project*> projectExported;
    
    sigslot::Signal2<Project*, ProjectRecord*> recordAdded;
    sigslot::Signal2<Project*, ProjectRecord*> recordRemoved;
    
private:
    bool OpenConfig(const std::string& filename);
    
    typedef std::set<Uid> UidSet;
    UidSet _Uids;
    
    std::string _Location;
    std::string _Name;
    
    RecordMap _Records;
    
    bool _IsOpen;
    Schema* _Schema;
    
    ProjectConfig _Config;
};
    
}
