#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "sigslot/signal.h"

#include "project/definitions.h"
#include "project/struct.h"

namespace json
{
    class Object;
}

namespace pixeleditor
{

class ProjectRecord;
class Property;
class PropertyAtom;
class PropertyArray;
class PropertyPointer;
class SchemaEntity;
        
class ProjectEntity : public ProjectStruct
{
public:
    ProjectEntity(ProjectRecord* record, const SchemaEntity* type = 0);
    virtual ~ProjectEntity();
    
    bool Open(json::Object& entity, bool skipUid = false);
    bool Save(json::Object& entity);
    
    bool ExportJson(json::Object& entity);
    bool ExportLua(std::iostream& output);
    
    virtual ProjectRecord* GetRecord();
    virtual const ProjectRecord* GetRecord() const;
    
public:    
    const glm::vec3& GetPosition();
    void SetPosition(const glm::vec3& position);
    
    float GetRotation();
    void SetRotation(float rotation);
    
    const glm::vec3& GetScale();
    void SetScale(const glm::vec3& scale);
    
    sigslot::Signal1<ProjectEntity*> entityChanged;
    
private:
    bool WriteTransformDataJson(json::Object& entity);
    bool WriteTransformDataLua(std::iostream& output);
    
private:
    ProjectRecord* _Record;
    
    glm::vec3 _Position;
    float _Rotation;
    glm::vec3 _Scale;
};
    
}
