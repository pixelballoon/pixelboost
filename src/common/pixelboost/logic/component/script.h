#pragma once

#include <map>
#include <set>
#include <string>

#include "pixelboost/db/types/components.h"
#include "pixelboost/logic/component.h"

struct lua_State;

namespace pb
{
    
    class LuaScript;
    class Message;
    
    class ScriptComponent : public Component
    {
        PB_DECLARE_COMPONENT
        
    protected:
        ScriptComponent(Entity* parent, const ScriptComponentDefinition* definition = 0);
        virtual ~ScriptComponent();
        
    public:
        static void RegisterLuaClass(lua_State* state);
        
        void SetActive(bool active);
        
        void SetSourceFile(const std::string& file);
        void SetSourceString(const std::string& source);
        
    private:
        void OnMessage(const pb::Message& message);
        void OnUpdate(const pb::Message& message);
        
        bool UpdateState(int threadIndex, float gameDelta);
        
        void LuaSendMessage(const Message* message);
        void LuaSendMessageTarget(Uid target, const Message* message);
        void LuaWaitMessage(const std::string& message, lua_State* state);
        void LuaWaitSeconds(float seconds, lua_State* state);
        
        int AddThread(lua_State* state);

        LuaScript* _Script;
        
        bool _Active;
        std::map<int, Uid> _WaitMessage;
        std::map<int, float> _WaitTime;
        std::map<lua_State*, int> _Threads;
        
        bool _ScriptYield;
    };

}
