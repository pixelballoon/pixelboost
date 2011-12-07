#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "pixelboost/network/networkHelpers.h"

namespace pixelboost
{
    
namespace NetworkHelpers
{

std::string GetWifiAddress()
{
#if TARGET_IPHONE_SIMULATOR
    return "127.0.0.1";
#else
    std::string address = "WiFi not enabled"; 
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    
    int success;
    success = getifaddrs(&interfaces); 
    
    if (success == 0)
    { 
        // Loop through linked list of interfaces  
        temp_addr = interfaces; 
        while(temp_addr != NULL)
        {
            if(temp_addr->ifa_addr->sa_family == AF_INET)
            {
                // en0 = iPhone WiFi connection
                if(strcmp(temp_addr->ifa_name, "en0") == 0)
                {
                    address = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr);
                } 
            }
            temp_addr = temp_addr->ifa_next; 
        } 
    }
    
    freeifaddrs(interfaces); 
    return address;
#endif
}

}
    
}
