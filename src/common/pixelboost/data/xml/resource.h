#pragma once

#include <string>

#include "pixelboost/db/definitions.h"
#include "pixelboost/data/xml/pugixml.hpp"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resource.h"

namespace pb
{

class XmlResource : public pb::Resource
{
public:
    XmlResource();
    ~XmlResource();
    
    virtual Uid GetResourceType();
	static Uid GetStaticResourceType();
    
    virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error);
    static ResourceThread GetResourceThread(ResourceState state);
    
    const pugi::xml_document& GetXmlDocument();
    
private:
    std::string _Data;
    pugi::xml_document _Document;
};

}
