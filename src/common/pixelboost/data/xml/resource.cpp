#include "pixelboost/data/xml/resource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"

using namespace pb;

XmlResource::XmlResource()
{
    
}

XmlResource::~XmlResource()
{
    
}

Uid XmlResource::GetResourceType()
{
    return GetStaticResourceType();
}

Uid XmlResource::GetStaticResourceType()
{
    return pb::TypeHash("pb::XmlResource");
}

bool XmlResource::ProcessResource(ResourceState state, const std::string& filename, std::string& error)
{
    switch (state)
    {
        case kResourceStateLoading:
        {
            pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
            if (!file)
            {
                PbLogError("pb.resource.xml", "Error opening XML file (%s)", filename.c_str());
                return false;
            }
            
            file->ReadAll(_Data);
            delete file;
            
            return true;
        }
            
        case kResourceStateProcessing:
        {
            pugi::xml_parse_result result = _Document.load(_Data.c_str());
            if (result.status != pugi::status_ok)
            {
                PbLogError("pb.resource.xml", "Error parsing XML file (%s)", result.description());
                error = result.description();
                return false;
            }
            
            return true;
        }
            
        case kResourceStatePostProcessing:
        case kResourceStateError:
        case kResourceStateComplete:
            return true;
            
    }
}

ResourceThread XmlResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadAny;
}

const pugi::xml_document& XmlResource::GetXmlDocument()
{
    return _Document;
}
