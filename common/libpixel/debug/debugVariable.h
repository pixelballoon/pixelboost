#pragma once

#include <string>

namespace libpixel
{
    
#ifndef PIXELBALLOON_BUILD_RELEASE
    
class DebugVariable
{
public:
    DebugVariable(const char* name);
    virtual ~DebugVariable();
    
    int GetId() const;
    const char* GetName() const;
    
public:
    enum VariableType
    {
        kVariableTypeBool,
        kVariableTypeString,
        kVariableTypeInteger,
        kVariableTypeFloat,
        kVariableTypeColor,
        kVariableTypeFunction,
    };
    
    virtual VariableType GetVariableType() = 0;
    
    virtual bool HasVariableChanged();
    
protected:
    void OnVariableChanged();
    
private:
    int _Id;
    
    char* _Name;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugBool : public DebugVariable
{
public:
    DebugBool(const char* name, bool value);
    
    virtual VariableType GetVariableType();
    
    DebugBool& operator =(bool value);
    operator bool() const;
    
    virtual bool HasVariableChanged();
    
private:
    bool _Original;
    bool _Value;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugString : public DebugVariable
{
public:
    DebugString(const char* name, const char* value);
    virtual ~DebugString();
    
    virtual VariableType GetVariableType();
    
    DebugString& operator =(const char* value);
    operator const char*() const;
    
    virtual bool HasVariableChanged();
    
private:
    std::string _Original;
    std::string _Value;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugInteger : public DebugVariable
{
public:
    DebugInteger(const char* name, int value, int min, int max);
    
    virtual VariableType GetVariableType();
    
    DebugInteger& operator =(int value);
    operator int() const;
    
    virtual bool HasVariableChanged();
    
private:
    int _Original;
    int _Value;
    int _Min;
    int _Max;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugFloat : public DebugVariable
{
public:
    DebugFloat(const char* name, float value, float min, float max);
    
    virtual VariableType GetVariableType();
    
    DebugFloat& operator =(float value);
    operator float() const;
    
    virtual bool HasVariableChanged();
    
private:
    float _Original;
    float _Value;
    float _Min;
    float _Max; 
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugColor : public DebugVariable
{
public:
    DebugColor(const char* name, float r, float g, float b, float a);
    
    virtual VariableType GetVariableType();
    
    float R();
    float G();
    float B();
    float A();
    
    void SetColor(float r, float g, float b, float a);
    void R(float r);
    void G(float g);
    void B(float b);
    void A(float a);
    
    virtual bool HasVariableChanged();
    
private:
    float _OriginalR;
    float _OriginalG;
    float _OriginalB;
    float _OriginalA;
    float _R;
    float _G;
    float _B;
    float _A;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};

class DebugFunction : public DebugVariable
{
public:
    DebugFunction(const char* name, void(*callback)(void* userData));
    
    virtual VariableType GetVariableType();
    
    void SetUserData(void* userData);
    
private:
    typedef void(*DebugFunctionCallback)(void*);
    DebugFunctionCallback _Callback;
    void* _UserData;
    
    friend class DebugVariableManager;
    friend class DebugVariableHandler;
};
    
#else
    
class DebugColor
{
public:
    DebugColor(float r, float g, float b, float a)
    {
        _R = r;
        _G = g;
        _B = b;
        _A = a;
    }
    
    float R()
    {
        return _R;
    }
    
    float G()
    {
        return _G;
    }
    
    float B()
    {
        return _B;
    }
    
    float A()
    {
        return _A;
    }
    
    void SetColor(float r, float g, float b, float a)
    {
        _R = r;
        _G = g;
        _B = b;
        _A = a;
    }
    
    void R(float r)
    {
        _R = r;
    }
    
    void G(float g)
    {
        _G = g;
    }
    
    void B(float b)
    {
        _B = b;
    }
    
    void A(float a)
    {
        _A = a;
    }
    
private:
    float _R;
    float _G;
    float _B;
    float _A;
};

class DebugFunction
{
public:
    DebugFunction() {}
    void SetUserData(void* userData) {}
};
    
#endif
}

#ifndef PIXELBALLOON_BUILD_RELEASE

#define DEFINE_DEBUG_STRING(name, location, value) libpixel::DebugString name(location, value)
#define DEFINE_DEBUG_BOOL(name, location, value) libpixel::DebugBool name(location, value)
#define DEFINE_DEBUG_INTEGER(name, location, value, min, max) libpixel::DebugInteger name(location, value, min, max)
#define DEFINE_DEBUG_FLOAT(name, location, value, min, max) libpixel::DebugFloat name(location, value, min, max)
#define DEFINE_DEBUG_COLOR(name, location, r, g, b, a) libpixel::DebugColor name(location, r, g, b, a)
#define DEFINE_DEBUG_FUNCTION(name, location, callback) libpixel::DebugFunction name(location, callback)

#define EXTERN_DEBUG_STRING(name) extern libpixel::DebugString name
#define EXTERN_DEBUG_BOOL(name) extern libpixel::DebugBool name
#define EXTERN_DEBUG_INTEGER(name) extern libpixel::DebugInteger name
#define EXTERN_DEBUG_FLOAT(name) extern libpixel::DebugFloat name
#define EXTERN_DEBUG_COLOR(name) extern libpixel::DebugColor name
#define EXTERN_DEBUG_FUNCTION(name) extern libpixel::DebugFunction name

#else

#define DEFINE_DEBUG_STRING(name, value, location) const char* name=value
#define DEFINE_DEBUG_BOOL(name, value, location) bool name=value;
#define DEFINE_DEBUG_INTEGER(name, value, location, min, max) int name=value
#define DEFINE_DEBUG_FLOAT(name, value, location, min, max) float name=value
#define DEFINE_DEBUG_COLOR(name, location, r, g, b, a) libpixel::DebugColor name(location, r, g, b, a)
#define DEFINE_DEBUG_FUNCTION(name, location, callback) libpixel::DebugFunction name()

#define EXTERN_DEBUG_STRING(name) extern const char* name
#define EXTERN_DEBUG_BOOL(name) extern bool name
#define EXTERN_DEBUG_INTEGER(name) extern int name
#define EXTERN_DEBUG_FLOAT(name) extern float name
#define EXTERN_DEBUG_COLOR(name) extern libpixel::DebugColor name
#define EXTERN_DEBUG_FUNCTION(name) extern libpixel::DebugFunction name

#endif
