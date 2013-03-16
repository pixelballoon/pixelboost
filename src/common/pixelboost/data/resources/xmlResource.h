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
        virtual bool ProcessResource(ResourceState state, const std::string& filename, std::string& error);
        virtual ResourceThread GetResourceThread(ResourceState state);
        
        const pugi::xml_document& GetXmlDocument();
        
    private:
        std::string _Data;
        pugi::xml_document _Document;
    };

}
