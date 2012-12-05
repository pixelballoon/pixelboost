#pragma once

#include "Rocket/Core.h"

namespace pb
{
    
class RocketSystem : public Rocket::Core::SystemInterface
{
    virtual float GetElapsedTime();
	virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);
};

}
