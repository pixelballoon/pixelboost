#pragma once

#include <string>
#include <vector>

#include "project/definitions.h"

namespace pixeleditor
{

class PropertyAtom;
class PropertyArray;
class PropertyPointer;
class PropertyReference;
class SchemaProperty;
class Struct;
    
class Property
{
public:
    Property(ProjectStruct* s, const SchemaProperty* schemaProperty);
    virtual ~Property();
    
    const SchemaProperty* GetSchemaProperty();
    
public:
    enum PropertyType
    {
        kPropertyArray,
        kPropertyAtom,
        kPropertyPointer,
        kPropertyReference,
    };
    
    virtual PropertyType GetType() const = 0;
    
public:
    virtual PropertyAtom* AsAtom();
    virtual const PropertyAtom* AsAtom() const;
    virtual PropertyArray* AsArray();
    virtual const PropertyArray* AsArray() const;
    virtual PropertyPointer* AsPointer();
    virtual const PropertyPointer* AsPointer() const;
    virtual PropertyReference* AsReference();
    virtual const PropertyReference* AsReference() const;
    
protected:
    const SchemaProperty* _SchemaProperty;
    
    ProjectStruct* _Struct;
};

class PropertyAtom : public Property
{
public:
    PropertyAtom(ProjectStruct* s, const SchemaProperty* schemaProperty);
    virtual ~PropertyAtom();
    
    virtual PropertyType GetType() const;
    
    virtual PropertyAtom* AsAtom();
    virtual const PropertyAtom* AsAtom() const;
    
    void SetFloatValue(float value);
    void SetIntValue(int value);
    void SetStringValue(const std::string& value);
    
    float GetFloatValue() const;
    int GetIntValue() const;
    const std::string& GetStringValue() const;
    
protected:
    bool _HasValue;
    std::string _Value;
};
    
class PropertyPointer : public Property
{
public:
    PropertyPointer(ProjectStruct* s, const SchemaProperty* schemaProperty);
    virtual ~PropertyPointer();
    
    virtual PropertyType GetType() const;
    
    virtual PropertyPointer* AsPointer();
    virtual const PropertyPointer* AsPointer() const;
    
    Uid GetPointerValue() const;
    void SetPointerValue(Uid uid);
    
    ProjectEntity* ResolvePointer() const;
    
protected:
    bool _HasValue;
    Uid _Value;
};

class PropertyReference : public Property
{
public:
    PropertyReference(ProjectStruct* s, const SchemaProperty* schemaProperty);
    virtual ~PropertyReference();
    
    virtual PropertyType GetType() const;
    
    virtual PropertyReference* AsReference();
    virtual const PropertyReference* AsReference() const;
    
    Uid GetReferenceValue() const;
    void SetReferenceValue(Uid uid);
    
    ProjectRecord* ResolveReference() const;
    
protected:
    bool _HasValue;
    Uid _Value;
};
    
class PropertyArray : public Property
{
public:
    PropertyArray(ProjectStruct* s, const SchemaProperty* schemaProperty);
    virtual ~PropertyArray();
    
    virtual PropertyType GetType() const;
    
    virtual PropertyArray* AsArray();
    virtual const PropertyArray* AsArray() const;
    
    unsigned int GetElementCount() const;
    unsigned int GetElementIdByIndex(unsigned int index) const;
    
    bool DoesContainElement(unsigned int elementId) const;
    
    unsigned int AddElement(unsigned int elementId=0);
    void RemoveElementById(unsigned int elementId);
    void RemoveElementByIndex(unsigned int index);
    
private:
    std::vector<unsigned int> _ArrayElements;
};

}
