#include <cstdio>

#include "debugVariable.h"
#include "debugVariableManager.h"

#ifndef PIXELBALLOON_BUILD_RELEASE

namespace libpixel
{

DebugVariable::DebugVariable(const char* name)
{
    _Name = new char[strlen(name)+1];
    strcpy(_Name, name);
    
    DebugVariableManager::Instance()->AddVariable(this);
}
    
DebugVariable::~DebugVariable()
{
    delete[] _Name;
}

const char* DebugVariable::GetName() const
{
    return _Name;
}
    
void DebugVariable::VariableChanged()
{
    DebugVariableManager::Instance()->VariableChanged(this);
}

DebugBool::DebugBool(const char* name, bool value)
    : DebugVariable(name)
    , _Value(value)
{
    
}
    
DebugVariable::VariableType DebugBool::GetVariableType()
{
    return kVariableTypeBool;
}
    
DebugBool& DebugBool::operator=(bool value)
{
    _Value = value;
    VariableChanged();
    return *this;
}
    
DebugBool::operator bool() const
{
    return _Value;
}
    
DebugString::DebugString(const char* name, const char* value)
    : DebugVariable(name)
    , _Value(0)
{
    SetValue(value);
}
    
DebugString::~DebugString()
{
    delete[] _Value;
}
    
DebugVariable::VariableType DebugString::GetVariableType()
{
    return kVariableTypeString;
}
    
DebugString& DebugString::operator=(const char* value)
{
    SetValue(value);
    
    VariableChanged();
    return *this;
}

DebugString::operator const char*() const
{
    return _Value;
}
    
void DebugString::SetValue(const char* value)
{
    int len = static_cast<int>(strlen(value));
    
    delete[] _Value;
    _Value = new char[len+1];
    strcpy(_Value, value);
}

DebugInteger::DebugInteger(const char* name, int value, int min, int max)
    : DebugVariable(name)
    , _Value(value)
    , _Min(min)
    , _Max(max)
{
    
}
    
DebugVariable::VariableType DebugInteger::GetVariableType()
{
    return kVariableTypeInteger;
}

DebugInteger& DebugInteger::operator=(int value)
{
    _Value = value;
    VariableChanged();
    return *this;
}
    
DebugInteger::operator int() const
{
    return _Value;
}

DebugFloat::DebugFloat(const char* name, float value, float min, float max)
    : DebugVariable(name)
, _Value(value)
, _Min(min)
, _Max(max)
{
    
}
    
DebugVariable::VariableType DebugFloat::GetVariableType()
{
    return kVariableTypeFloat;
}

DebugFloat& DebugFloat::operator=(float value)
{
    _Value = value;
    VariableChanged();
    return *this;
}
    
DebugFloat::operator float() const
{
    return _Value;
}
    
DebugColour::DebugColour(const char* name, float r, float g, float b, float a)
    : DebugVariable(name)
    , _R(r)
    , _G(g)
    , _B(b)
    , _A(a)
{
    
}

DebugVariable::VariableType DebugColour::GetVariableType()
{
    return kVariableTypeColour;
}

float DebugColour::R()
{
    return _R;
}
    
float DebugColour::G()
{
    return _G;
}
    
float DebugColour::B()
{
    return _B;
}
    
float DebugColour::A()
{
    return _A;
}
    
void DebugColour::SetColour(float r, float g, float b, float a)
{
    _R = r;
    _G = g;
    _B = b;
    _A = a;
    VariableChanged();
}

void DebugColour::R(float r)
{
    _R = r;
    VariableChanged();
}
    
void DebugColour::G(float g)
{
    _G = g;
    VariableChanged();
}
    
void DebugColour::B(float b)
{
    _B = b;
    VariableChanged();
}
    
void DebugColour::A(float a)
{
    _A = a;
    VariableChanged();
}
    
DebugFunction::DebugFunction(const char* name, void(*callback)(void*))
    : DebugVariable(name)
{
    _Callback = callback;
}

    
DebugVariable::VariableType DebugFunction::GetVariableType()
{
    return kVariableTypeFunction;
}
    
void DebugFunction::SetUserData(void* userData)
{
    _UserData = userData;
}

}

#endif