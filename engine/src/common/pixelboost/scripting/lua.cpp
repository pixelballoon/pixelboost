#include "lualib.h"

#include "pixelboost/framework/engine.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

PB_LUA_DECLARE(glm)
PB_LUA_DECLARE(pixelboost)

LuaScript::LuaScript(ScriptComponent* component, lua_State* state)
{
    _Component = component;
    _State = state;
}

LuaScript::~LuaScript()
{
    lua_close(_State);
}

lua_State* LuaScript::GetState()
{
    return _State;
}

void LuaScript::Load(const std::string& lua)
{
    luaL_loadstring(_State, lua.c_str());
    
    luabridge::push(_State, Engine::Instance());
    lua_setglobal(_State, "engine");
    
    // Override _ENV with custom environment, containing references to the script
    lua_newtable(_State);
    
    lua_newtable(_State);
    lua_getglobal(_State, "_G");
    lua_setfield(_State, -2, "__index");
    lua_setmetatable(_State, -2);
    
    luabridge::push(_State, _Component);
    lua_setfield(_State, -2, "script");

    lua_setupvalue(_State, -2, 1);
}

void LuaScript::Step()
{
    lua_resume(_State, 0, 0);
}

LuaManager::LuaManager()
{
    _State = luaL_newstate();
    luaL_openlibs(_State);
    
    PB_LUA_REGISTER(this, glm);
    PB_LUA_REGISTER(this, pixelboost);
}

LuaManager::~LuaManager()
{

}

LuaManager* LuaManager::Instance()
{
    static LuaManager* _Instance = new LuaManager();
    return _Instance;
}

void LuaManager::RegisterClass(ClassRegistration function)
{
    _Registration.push_back(function);
    
    function(_State);
}

LuaScript* LuaManager::CreateScript(ScriptComponent* scene)
{
    lua_State* state = lua_newthread(_State);
    
    LuaScript* script = new LuaScript(scene, state);
    
    return script;
}
