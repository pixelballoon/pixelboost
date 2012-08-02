#ifdef PIXELBOOST_PLATFORM_ANDROID

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "pixelboost/network/networkHelpers.h"

namespace pb
{
    
namespace NetworkHelpers
{

std::string GetWifiAddress()
{
    return "";
}

}
    
}

#endif