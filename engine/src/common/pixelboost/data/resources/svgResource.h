#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "pugixml/pugixml.hpp"

#include "pixelboost/maths/hermiteCurve.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{
    
    struct SvgPath
    {
        std::string Name;
        
        HermiteCurve2D Curve;
    };
    
    struct SvgGroup
    {
        std::vector<SvgPath> Paths;
    };

    class SvgResource : public Resource
    {
        PB_DECLARE_RESOURCE
        
    public:
        SvgResource(ResourcePool* pool, const std::string& filename);
        ~SvgResource();
        
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);

        const std::map<std::string, SvgGroup>& GetGroups();
        glm::vec2 GetSize();
        
    private:
        ResourceError Load(const std::string& filename);
        
        bool ParseAll();
        bool ParseGroup(const std::string& name);
        
        glm::vec2 _Size;
        
        std::map<std::string, SvgGroup> _Groups;
        
        pugi::xml_document _Xml;
    };

}
