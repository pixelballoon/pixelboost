#include "pixelboost/scripting/lua.h"

#include "pixelboost/framework/engine.h"
#include "pixelboost/logic/component/script.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/message.h"
#include "pixelboost/logic/scene.h"

static void PbRegisterGlm(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginClass<glm::vec2>("vec2")
    .addConstructor<void (*) (float, float)>()
        .addData("x", &glm::vec2::x)
        .addData("y", &glm::vec2::y)
    .endClass()
    .beginClass<glm::vec3>("vec3")
        .addConstructor<void (*) (float, float, float)>()
        .addData("x", &glm::vec3::x)
        .addData("y", &glm::vec3::y)
        .addData("z", &glm::vec3::z)
    .endClass()
    .beginClass<glm::vec4>("vec4")
        .addConstructor<void (*) (float, float, float, float)>()
        .addData("x", &glm::vec4::x)
        .addData("y", &glm::vec4::y)
        .addData("z", &glm::vec4::z)
        .addData("w", &glm::vec4::w)
    .endClass()
    .beginClass<glm::vec4>("color")
        .addConstructor<void (*) (float, float, float, float)>()
        .addData("r", &glm::vec4::r)
        .addData("g", &glm::vec4::g)
        .addData("b", &glm::vec4::b)
        .addData("a", &glm::vec4::a)
    .endClass();
}

PB_LUA_BEGIN(glm)
    manager->RegisterClass(&PbRegisterGlm);
PB_LUA_END()

PB_LUA_BEGIN(pixelboost)
    PB_LUA_DEFINE_CLASS(pb::Engine)

    PB_LUA_DEFINE_CLASS(pb::Entity)
    PB_LUA_DEFINE_CLASS(pb::Message)
    PB_LUA_DEFINE_CLASS(pb::Scene)

    PB_LUA_DEFINE_CLASS(pb::Component)
    PB_LUA_DEFINE_CLASS(pb::ScriptComponent)
PB_LUA_END()
