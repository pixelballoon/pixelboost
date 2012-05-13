#include <cstdio>

#include "debugVariable.h"
#include "debugVariableManager.h"

#ifndef PIXELBOOST_BUILD_RELEASE

namespace pb
{

DebugVariable::DebugVariable(const char* name)
{
    static int freeId = 0;
    
    _Id = freeId++;
    _Name = new char[strlen(name)+1];
    strcpy(_Name, name);
    
    DebugVariableManager::Instance()->AddVariable(this);
}
    
DebugVariable::~DebugVariable()
{
    delete[] _Name;
}
    
int DebugVariable::GetId() const
{
    return _Id;
}

const char* DebugVariable::GetName() const
{
    return _Name;
}
    
bool DebugVariable::HasVariableChanged()
{
    return false;
}
    
void DebugVariable::Reset()
{
}
    
void DebugVariable::OnVariableChanged()
{
}

DebugBool::DebugBool(const char* name, bool value)
    : DebugVariable(name)
    , _Original(value)
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
    OnVariableChanged();
    return *this;
}
    
DebugBool::operator bool() const
{
    return _Value;
}
    
bool DebugBool::HasVariableChanged()
{
    return _Original != _Value;
}

void DebugBool::Reset()
{
    _Value = _Original;
}
    
DebugString::DebugString(const char* name, const char* value)
    : DebugVariable(name)
    , _Original(value)
    , _Value(value)
{
}
    
DebugString::~DebugString()
{
}
    
DebugVariable::VariableType DebugString::GetVariableType()
{
    return kVariableTypeString;
}
    
DebugString& DebugString::operator=(const char* value)
{
    _Value = value;
    
    OnVariableChanged();
    return *this;
}

DebugString::operator const char*() const
{
    return _Value.c_str();
}
    
bool DebugString::HasVariableChanged()
{
    return _Original != _Value;
}
    
void DebugString::Reset()
{
    _Value = _Original;
}

DebugInteger::DebugInteger(const char* name, int value, int min, int max)
    : DebugVariable(name)
    , _Original(value)
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
    OnVariableChanged();
    return *this;
}
    
DebugInteger::operator int() const
{
    return _Value;
}
    
bool DebugInteger::HasVariableChanged()
{
    return _Original != _Value;
}
    
void DebugInteger::Reset()
{
    _Value = _Original;
}

DebugFloat::DebugFloat(const char* name, float value, float min, float max)
    : DebugVariable(name)
    , _Original(value)
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
    OnVariableChanged();
    return *this;
}
    
DebugFloat::operator float() const
{
    return _Value;
}
    
bool DebugFloat::HasVariableChanged()
{
    return _Original != _Value;
}
    
void DebugFloat::Reset()
{
    _Value = _Original;
}
    
DebugColor::DebugColor(const char* name, float r, float g, float b, float a)
    : DebugVariable(name)
    , _R(r)
    , _G(g)
    , _B(b)
    , _A(a)
    , _OriginalR(r)
    , _OriginalG(g)
    , _OriginalB(b)
    , _OriginalA(a)
{
    
}

DebugVariable::VariableType DebugColor::GetVariableType()
{
    return kVariableTypeColor;
}

float DebugColor::R()
{
    return _R;
}
    
float DebugColor::G()
{
    return _G;
}
    
float DebugColor::B()
{
    return _B;
}
    
float DebugColor::A()
{
    return _A;
}
    
void DebugColor::SetColor(float r, float g, float b, float a)
{
    _R = r;
    _G = g;
    _B = b;
    _A = a;
    OnVariableChanged();
}

void DebugColor::R(float r)
{
    _R = r;
    OnVariableChanged();
}
    
void DebugColor::G(float g)
{
    _G = g;
    OnVariableChanged();
}
    
void DebugColor::B(float b)
{
    _B = b;
    OnVariableChanged();
}
    
void DebugColor::A(float a)
{
    _A = a;
    OnVariableChanged();
}
    
bool DebugColor::HasVariableChanged()
{
    return _OriginalR != _R || _OriginalG != _G || _OriginalB != _B || _OriginalA != _A;
}
    
void DebugColor::Reset()
{
    _R = _OriginalR;
    _G = _OriginalG;
    _B = _OriginalB;
    _A = _OriginalA;
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
