#ifdef PIXELBOOST_PLATFORM_IOS

#include "ResourceManager.h"

namespace pixelboost
{

AsyncResource::AsyncResource(const std::string& fileName)
{
}

bool AsyncResource::IsReady()
{
	if (_Status == E_STATUS_READY)
		return true;
	
	return false;
}

void AsyncResource::Notify(Status status)
{
}


std::string ResourceManager::GetFilename(const std::string& fileName)
{
	NSString* string = [[NSBundle mainBundle] pathForResource:[NSString stringWithCString:fileName.c_str() encoding:[NSString defaultCStringEncoding]] ofType:@""];
	return [string cStringUsingEncoding:[NSString defaultCStringEncoding]];
}

}

#endif