#pragma once

#include "pixelboost/framework/definitions.h"

struct lua_State;

#define PB_DECLARE_MESSAGE public: virtual pb::Uid GetType() const; static pb::Uid GetStaticType(); private:
#define PB_DEFINE_MESSAGE(className) pb::Uid className::GetType() const { return GetStaticType(); } pb::Uid className::GetStaticType() { return pb::TypeHash(#className); }

namespace pb
{

    class Component;
    class Entity;

    class Message
    {
    public:
        Message(Entity* entity, Component* component);
        virtual ~Message();
        
        static void RegisterLuaClass(lua_State* state);
        
        template<class T> const T& As() const;
        
        virtual Uid GetType() const;
        static Uid GetStaticType();        
        
        Entity* GetEntity() const;
        Component* GetComponent() const;
        
    private:
        Entity* _Entity;
        Component* _Component;
    };
    
}

#include "pixelboost/logic/message.inl"
