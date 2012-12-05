#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/ui/rocket.h"

using namespace pb;

float RocketSystem::GetElapsedTime()
{
    return Engine::Instance()->GetGameTime();
}

bool RocketSystem::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    PbLogDebug("ui.rocket", "%s\n", message.CString());
    return true;
}
