#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/struct.h"

using namespace pixeleditor;

Property::Property(ProjectStruct* s, const SchemaProperty* schemaProperty)
    : _Struct(s)
    , _SchemaProperty(schemaProperty)
{
    
}

Property::~Property()
{
    
}

const SchemaProperty* Property::GetSchemaProperty()
{
    return _SchemaProperty;
}

PropertyAtom* Property::AsAtom()
{
    return 0;
}

const PropertyAtom* Property::AsAtom() const
{
    return 0;
}

PropertyArray* Property::AsArray()
{
    return 0;
}

const PropertyArray* Property::AsArray() const
{
    return 0;
}
    
PropertyPointer* Property::AsPointer()
{
    return 0;
}

const PropertyPointer* Property::AsPointer() const
{
    return 0;
}

PropertyReference* Property::AsReference()
{
    return 0;
}

const PropertyReference* Property::AsReference() const
{
    return 0;
}

PropertyAtom::PropertyAtom(ProjectStruct* s, const SchemaProperty* schemaProperty)
    : Property(s, schemaProperty)
{
    
}

PropertyAtom::~PropertyAtom()
{
    
}

Property::PropertyType PropertyAtom::GetType() const
{
    return kPropertyAtom;
}

PropertyAtom* PropertyAtom::AsAtom()
{
    return this;
}

const PropertyAtom* PropertyAtom::AsAtom() const
{
    return this;
}

void PropertyAtom::SetFloatValue(float value)
{
    char tmp[64];
    snprintf(tmp, 64, "%f", value);
    _Value = tmp;
    _Struct->propertyChanged(_Struct, this);
}

void PropertyAtom::SetIntValue(int value)
{
    char tmp[64];
    snprintf(tmp, 64, "%d", value);
    _Value = tmp;
    _Struct->propertyChanged(_Struct, this);
}

void PropertyAtom::SetStringValue(const std::string& value)
{
    _Value = value;
    _Struct->propertyChanged(_Struct, this);
}

float PropertyAtom::GetFloatValue() const
{
    return atof(_Value.c_str());
}

int PropertyAtom::GetIntValue() const
{
    return atoi(_Value.c_str());
}

const std::string& PropertyAtom::GetStringValue() const
{
    return _Value;
}


PropertyPointer::PropertyPointer(ProjectStruct* s, const SchemaProperty* schemaProperty)
    : Property(s, schemaProperty)
{
    
}

PropertyPointer::~PropertyPointer()
{
    
}

Property::PropertyType PropertyPointer::GetType() const
{
    return kPropertyPointer;
}

PropertyPointer* PropertyPointer::AsPointer()
{
    return this;
}

const PropertyPointer* PropertyPointer::AsPointer() const
{
    return this;
}

Uid PropertyPointer::GetPointerValue() const
{
    return _Value;
}

void PropertyPointer::SetPointerValue(Uid uid)
{
    _Value = uid;
    _Struct->propertyChanged(_Struct, this);
}

ProjectEntity* PropertyPointer::ResolvePointer() const
{
    Project* project = _Struct->GetProject();
    
    return project->GetEntity(_Value);
}

PropertyReference::PropertyReference(ProjectStruct* s, const SchemaProperty* schemaProperty)
    : Property(s, schemaProperty)
{
    
}

PropertyReference::~PropertyReference()
{
    
}

Property::PropertyType PropertyReference::GetType() const
{
    return kPropertyReference;
}

PropertyReference* PropertyReference::AsReference()
{
    return this;
}

const PropertyReference* PropertyReference::AsReference() const
{
    return this;
}

Uid PropertyReference::GetReferenceValue() const
{
    return _Value;
}

void PropertyReference::SetReferenceValue(Uid uid)
{
    _Value = uid;
    _Struct->propertyChanged(_Struct, this);
}

ProjectRecord* PropertyReference::ResolveReference() const
{
    Project* project = _Struct->GetProject();
    
    return project->GetRecord(_Value);
}

PropertyArray::PropertyArray(ProjectStruct* s, const SchemaProperty* schemaProperty)
    : Property(s, schemaProperty)
{
    
}

PropertyArray::~PropertyArray()
{
    
}

Property::PropertyType PropertyArray::GetType() const
{
    return kPropertyArray;
}

PropertyArray* PropertyArray::AsArray()
{
    return this;
}

const PropertyArray* PropertyArray::AsArray() const
{
    return this;
}

unsigned int PropertyArray::GetElementCount() const
{
    return _ArrayElements.size();
}

unsigned int PropertyArray::GetElementIdByIndex(unsigned int index) const
{
    if (index < _ArrayElements.size())
        return _ArrayElements[index];
    
    return 0;
}

bool PropertyArray::DoesContainElement(unsigned int elementId) const
{
    for (std::vector<unsigned int>::const_iterator it = _ArrayElements.begin(); it != _ArrayElements.end(); ++it)
    {
        if (*it == elementId)
            return true;
    }
    
    return false;
}

unsigned int PropertyArray::AddElement(unsigned int elementId)
{
    if (elementId == 0)
    {
        elementId = rand();
        while (DoesContainElement(elementId))
            elementId = rand();
    }
    
    _ArrayElements.push_back(elementId);
    
    _Struct->propertyChanged(_Struct, this);
    
    return elementId;
}

unsigned int PropertyArray::AddElementBeforeIndex(unsigned int index, unsigned int elementId)
{
    if (elementId == 0)
    {
        elementId = rand();
        while (DoesContainElement(elementId))
            elementId = rand();
    }
    
    std::vector<unsigned int>::iterator it = _ArrayElements.begin() + index;
    _ArrayElements.insert(it, elementId);
    
    _Struct->propertyChanged(_Struct, this);
    
    return elementId;
}

void PropertyArray::RemoveElementById(unsigned int elementId)
{
    for (std::vector<unsigned int>::iterator it = _ArrayElements.begin(); it != _ArrayElements.end(); ++it)
    {
        if (*it == elementId)
        {
            _ArrayElements.erase(it);
            _Struct->propertyChanged(_Struct, this);
            return;
        }
    }
}

void PropertyArray::RemoveElementByIndex(unsigned int index)
{
    if (index >= _ArrayElements.size())
        return;
    
    std::vector<unsigned int>::iterator it = _ArrayElements.begin();
    
    for (int i = 0; i < index; i++)
        it++;
    
    _ArrayElements.erase(it);
    
    _Struct->propertyChanged(_Struct, this);
}
