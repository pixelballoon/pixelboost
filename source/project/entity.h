#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/math/maths.h"
#include "pixelboost/misc/delegate/signal.h"

#include "project/definitions.h"
#include "project/struct.h"

namespace json
{
    class Object;
}

namespace pixeleditor
{

class Property;
class PropertyAtom;
class PropertyArray;
class PropertyPointer;
class Record;
class SchemaEntity;
        
class Entity : public Struct
{
public:
    Entity(Record* record, const SchemaEntity* type = 0);
    virtual ~Entity();
    
    bool Open(json::Object& entity);
    bool Save(json::Object& entity);
    
    bool ExportJson(json::Object& entity);
    bool ExportLua(std::iostream& output);
    
    virtual Record* GetRecord();
    virtual const Record* GetRecord() const;
    
public:    
    const Vec3& GetPosition();
    void SetPosition(const Vec3& position);
    
    float GetRotation();
    void SetRotation(float rotation);
    
    const Vec3& GetScale();
    void SetScale(const Vec3& scale);
    
    sigslot::Signal1<Entity*> entityChanged;
    
private:
    bool WriteTransformDataJson(json::Object& entity);
    bool WriteTransformDataLua(std::iostream& output);
    
private:
    Record* _Record;
    
    Vec3 _Position;
    float _Rotation;
    Vec3 _Scale;
};
    
}
