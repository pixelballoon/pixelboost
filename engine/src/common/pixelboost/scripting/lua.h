#pragma once

#include <string>
#include <vector>

#include "lua.h"
#include "lauxlib.h"

#include "LuaBridge/LuaBridge.h"

#define PB_LUA_BEGIN(namespace) void PbRegisterLua_ ## namespace(pb::LuaManager* manager) {
#define PB_LUA_DEFINE_CLASS(class) manager->RegisterClass(& class ::RegisterLuaClass);
#define PB_LUA_END(name) }

#define PB_LUA_DECLARE(namespace) void PbRegisterLua_ ## namespace(pb::LuaManager* manager);
#define PB_LUA_REGISTER(manager, namespace) PbRegisterLua_ ## namespace(manager);

namespace pb
{
    
class ScriptComponent;
    
class LuaScript
{
private:
    LuaScript(ScriptComponent* script, lua_State* state);
    
public:
    ~LuaScript();
    
    lua_State* GetState();
    
    void Load(const std::string& lua);
    void Step();
    
private:
    lua_State* _State;
    ScriptComponent* _Component;
    
    friend class LuaManager;
};

class LuaManager
{
private:
    LuaManager();
    ~LuaManager();
    
public:
    static LuaManager* Instance();
    
public:
    LuaScript* CreateScript(ScriptComponent* script);
    
public:
    typedef void (*ClassRegistration)(lua_State* lua);
    void RegisterClass(ClassRegistration function);
    
private:
    std::vector<ClassRegistration> _Registration;
    
    lua_State* _State;
};

}
