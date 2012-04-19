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
    bool Export(json::Object& entity);
    
    virtual Record* GetRecord();
    virtual const Record* GetRecord() const;
    
public:    
    const Vec2& GetPosition();
    void SetPosition(const Vec2& position);
    
    float GetRotation();
    void SetRotation(float rotation);
    
    const Vec2& GetScale();
    void SetScale(const Vec2& scale);
    
    sigslot::Signal1<Entity*> entityChanged;
    
private:
    bool WriteTransformData(json::Object& entity);
    
private:
    Record* _Record;
    
    Vec2 _Position;
    float _Rotation;
    Vec2 _Scale;
};
    
}
