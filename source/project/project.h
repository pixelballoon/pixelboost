#pragma once

#include <map>
#include <set>
#include <string>

#include "sigslot/signal.h"

#include "project/definitions.h"

namespace pixeleditor
{

class Entity;
class Record;
class Schema;

class Project
{
public:
    Project();
    ~Project();
    
    bool Open(const std::string& directory);
    bool Close();
    bool Save();
    bool Export();
    
    bool IsOpen();
    
    const std::string& GetLocation() const;
    const std::string& GetName() const;
    
    Entity* GetEntity(Uid uid) const;
    Record* GetRecord(Uid uid) const;
    Record* GetRecordByName(const std::string& recordName) const;
    
    Uid CalculateUid(Uid min=65535, Uid max=0xffffffffUL);
    bool RegisterUid(Uid uid);
    bool ReleaseUid(Uid uid);

public:
    struct ProjectConfig
    {
        std::vector<std::string> imageRoots;
        std::string exportDir;
        unsigned int pixelUnit;
    };
    
    const ProjectConfig& GetConfig() const;
    
    Schema* GetSchema();
    
public:
    typedef std::map<Uid, Record*> RecordMap;
    
    const RecordMap& GetRecords() const;
    bool AddRecord(const std::string& name, const std::string& type);
    bool LoadRecord(const std::string& filename);
    bool RemoveRecord(const std::string& name, bool erase=true);    
    
public:
    sigslot::Signal1<Project*> projectOpened;
    sigslot::Signal1<Project*> projectClosed;
    sigslot::Signal1<Project*> projectSaved;
    sigslot::Signal1<Project*> projectExported;
    
    sigslot::Signal2<Project*, Record*> recordAdded;
    sigslot::Signal2<Project*, Record*> recordRemoved;
    
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
