#include "pixelboost/data/resources/xmlResource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::XmlResource)

XmlResource::XmlResource(ResourcePool* pool, const std::string& filename)
    : Resource(pool, filename)
{
    
}

XmlResource::~XmlResource()
{
    
}

ResourceError XmlResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            File* file = FileSystem::Instance()->OpenFile(filename);
            if (!file)
            {
                PbLogError("pb.resource.xml", "Error opening XML file (%s)", filename.c_str());
                return kResourceErrorNoSuchResource;
            }
            
            file->ReadAll(_Data);
            delete file;
            
            return kResourceErrorNone;
        }
            
        case kResourceProcessProcess:
        {
            pugi::xml_parse_result result = _Document.load(_Data.c_str());
            if (result.status != pugi::status_ok)
            {
                PbLogError("pb.resource.xml", "Error parsing XML file (%s)", result.description());
                errorDetails = result.description();
                return kResourceErrorSystemError;
            }
            
            return kResourceErrorNone;
        }
        
        case kResourceProcessPostProcess:
            return kResourceErrorNone;
            
        case kResourceProcessUnload:
            _Document.reset();
            return kResourceErrorNone;
    }
}

const pugi::xml_document& XmlResource::GetXmlDocument()
{
    return _Document;
}
