#pragma once

#ifdef PIXELBOOST_PLATFORM_WINDOWS
	#include <memory>
#else
	#include <tr1/memory>
#endif

namespace std
{
    using std::tr1::shared_ptr;
}
