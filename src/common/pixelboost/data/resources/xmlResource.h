#pragma once

#include <string>

#include "pugixml/pugixml.hpp"

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resource.h"

namespace pb
{

    class XmlResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        XmlResource();
        ~XmlResource();
        
    public:
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        
        const pugi::xml_document& GetXmlDocument();
        
    private:
        std::string _Data;
        pugi::xml_document _Document;
    };

}
