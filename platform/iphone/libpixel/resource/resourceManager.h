#ifndef LIBPIXEL__RESOURCE__RESOURCEMANAGER__H
#define LIBPIXEL__RESOURCE__RESOURCEMANAGER__H

#include <string>

namespace libpixel
{

class AsyncResource
{
public:
	AsyncResource(const std::string& fileName);
	
	enum Status
	{
		E_STATUS_READY,
		E_STATUS_PENDING,
		E_STATUS_ERROR_NO_FILE,		
	};
	
	bool IsReady();
	virtual void Notify(Status status);
	
private:
	Status _Status;
};

class ResourceManager
{
	std::string GetFilename(const std::string& fileName);
};
    
}

#endif