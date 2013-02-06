#include "pixelboost/framework/engine.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

PB_LUA_DECLARE(glm)
PB_LUA_DECLARE(pixelboost)

LuaScript::LuaScript(ScriptComponent* component, lua_State* state)
{
    _State = state;
    
    luabridge::push(_State, component);
    lua_setglobal(_State, "script");
    
    luabridge::push(_State, Engine::Instance());
    lua_setglobal(_State, "engine");
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
}

void LuaScript::Step()
{
    lua_resume(_State, 0, 0);
}

LuaManager::LuaManager()
{
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
}

LuaScript* LuaManager::CreateScript(ScriptComponent* scene)
{
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);

    for (std::vector<ClassRegistration>::iterator it = _Registration.begin(); it != _Registration.end(); ++it)
    {
        (*it)(state);
    }
    
    LuaScript* script = new LuaScript(scene, state);
    
    return script;
}
