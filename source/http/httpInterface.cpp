#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "pixelboost/data/json/writer.h"
#include "pixelboost/file/fileHelpers.h"

#include "command/manager.h"
#include "http/commands/file.h"
#include "http/commands/record.h"
#include "http/commands/schema.h"
#include "http/httpInterface.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "core.h"

using namespace pb;
using namespace pixeleditor;

HttpInterface::HttpInterface()
    : pb::HttpServer()
{
    
}

HttpInterface::~HttpInterface()
{
    
}

void HttpInterface::Initialise()
{
    _FileCommands = new FileCommands();
    _SchemaCommands = new SchemaCommands();
    _RecordCommands = new RecordCommands();
    
    Start(9090, pb::FileHelpers::GetRootPath()+"/data/html/");
}

void HttpInterface::RegisterCommand(const std::string& command, HttpServer::RequestType requestType, CommandDelegate delegate)
{
    _Commands[CommandRequest(command, requestType)] = delegate;
}

bool HttpInterface::OnHttpRequest(HttpServer::RequestType type, const std::string& uri, const std::string& query, const std::string& data, pb::HttpConnection& connection)
{
    ssize_t split = uri.find('/', 1);
    
    std::string command;
    std::string argumentString;
    std::string queryString = query;
    std::vector<std::string> urlArguments;
    std::map<std::string, std::string> queryArguments;
    
    if (split > 0)
    {
        command = uri.substr(1, split-1);
        argumentString = uri.substr(split);
    } else {
        command = uri.substr(1);
    }
        
    while (argumentString.length() > 1)
    {
        ssize_t split = argumentString.find('/', 1);
        
        std::string argument;
        
        if (split > 0)
        {
            argument = argumentString.substr(1, split-1);
            argumentString = argumentString.substr(split);
        } else {
            argument = argumentString.substr(1);
            argumentString = "";
        }
        
        if (argument.length() > 0)
            urlArguments.push_back(argument);
        else
            break;
    }
    
    while (queryString.length() > 1)
    {
        ssize_t split = queryString.find('&', 1);
        
        std::string argument;
        
        if (split > 0)
        {
            argument = queryString.substr(0, split);
            queryString = queryString.substr(split+1);
        } else {
            argument = queryString.substr(0);
            queryString = "";
        }
        
        if (argument.length() > 0)
        {
            std::string field;
            std::string value;
            
            int op = argument.find('=');
            if (op > 0)
            {
                field = argument.substr(0, op);
                value = argument.substr(op+1);
            }
            queryArguments[field] = value;
        }
        else
            break;
    }
    
    bool replied = false;
    
    CommandRequest request(command, type);
    
    CommandMap::iterator commandHandler = _Commands.find(request);
    if (commandHandler != _Commands.end())
    {
        connection.AddHeader("Content-Type", "application/json;charset=utf-8");
        replied = commandHandler->second(connection, urlArguments, queryArguments);
    } else {
    
        if (command == "images" && type == kRequestTypeGet)
        {
            connection.AddHeader("Content-Type", "image/png");
            
            if (urlArguments.size() == 1)
            {
                replied = OnGetImage(connection, urlArguments[0]);
            }
        } else {
            connection.AddHeader("Content-Type", "application/json;charset=utf-8");
        }
        
        if (command == "record" && type == kRequestTypeGet)
        {
            if (urlArguments.size() == 1)
            {
                Uid record = atoi(urlArguments[0].c_str());
                replied = OnGetRecord(connection, record);
            } else if (urlArguments.size() >= 2 && urlArguments[1] == "property")
            {
                Uid record = atoi(urlArguments[0].c_str());
                
                std::string path = "/";
                
                for (int i=2; i < urlArguments.size(); i++)
                {
                    path += urlArguments[i] + "/";
                }
                
                replied = OnGetRecordProperty(connection, record, path);
            } else if (urlArguments.size() >= 4 && urlArguments[1] == "entity" && urlArguments[3] == "property")
            {
                Uid record = atoi(urlArguments[0].c_str());
                Uid entity = atoi(urlArguments[2].c_str());
                
                std::string path = "/";
                
                for (int i=4; i < urlArguments.size(); i++)
                {
                    path += urlArguments[i] + "/";
                }
                
                replied = OnGetEntityProperty(connection, record, entity, path);
            }
        } else if (command == "record" && (type == kRequestTypePost || type == kRequestTypePut))
        {
            if (urlArguments.size() >= 2 && urlArguments[1] == "property")
            {
                Uid record = atoi(urlArguments[0].c_str());
                
                std::string type = queryArguments["type"];
                std::string value = queryArguments["value"];
                
                std::string path = "/";
                
                for (int i=2; i < urlArguments.size(); i++)
                {
                    path += urlArguments[i] + "/";
                }
                
                replied = OnSetRecordProperty(connection, record, path, type, value);
            } else if (urlArguments.size() == 2 && urlArguments[1] == "entities" && type == kRequestTypePost)
            {
                Uid record = atoi(urlArguments[0].c_str());
                replied = OnCreateEntity(connection, record, queryArguments["type"]);
            } else if (urlArguments.size() >= 4 && urlArguments[1] == "entity" && type == kRequestTypePut)
            {
                Uid record = atoi(urlArguments[0].c_str());
                Uid entity = atoi(urlArguments[2].c_str());
                
                if (urlArguments[3] == "transform")
                {
                    replied = OnSetTransform(connection, record, entity, Vec3(atof(queryArguments["tx"].c_str()), atof(queryArguments["ty"].c_str()), atof(queryArguments["tz"].c_str())), 0, Vec3(1,1,1));
                } else if (urlArguments[3] == "property")
                {
                    std::string type = queryArguments["type"];
                    std::string value = queryArguments["value"];
                    
                    std::string path = "/";
                    
                    for (int i=4; i < urlArguments.size(); i++)
                    {
                        path += urlArguments[i] + "/";
                    }
                    
                    replied = OnSetEntityProperty(connection, record, entity, path, type, value);
                }
            }
        } else if (command == "records" && type == kRequestTypePost)
        {
            if (urlArguments.size() == 0)
            {
                replied = OnCreateRecord(connection, queryArguments["name"], queryArguments["type"]);
            }
        }
    }
    
    if (replied == true)
    {
        connection.Send();
        return true;
    }
    
    if (type == kRequestTypeGet)
        return false; // No over-ride, default to finding html
    else
        return true;
}

bool HttpInterface::OnCreateRecord(pb::HttpConnection& connection, const std::string& name, const std::string& type)
{
    if (name == "")
        return false;
    
    Project* project = Core::Instance()->GetProject();
    
    project->AddRecord(name, type);
    
    return true;
}

bool HttpInterface::OnCreateEntity(pb::HttpConnection& connection, Uid recordId, const std::string& type)
{
    char arguments[256];
    sprintf(arguments, "-r %d -t %s -p 0,0", recordId, type.c_str());
    Core::Instance()->GetCommandManager()->Exec("createEntity", arguments);
    return true;
}

bool HttpInterface::OnSetTransform(pb::HttpConnection& connection, Uid recordId, Uid entityId, const Vec3& position, float rotation, const Vec3& scale)
{
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return false;
    
    entity->SetPosition(position);
    entity->SetRotation(rotation);
    entity->SetScale(scale);
    
    return true;
}

bool HttpInterface::OnGetRecord(pb::HttpConnection& connection, Uid recordId)
{
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    record->ExportJson(data);
    
    std::stringstream contentStream;
    json::Writer::Write(data, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetEntity(pb::HttpConnection& connection, Uid recordId, Uid entityId)
{
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return false;
    
    entity->ExportJson(data);
    
    std::stringstream contentStream;
    json::Writer::Write(data, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetRecordProperty(pb::HttpConnection& connection, Uid recordId, const std::string& path)
{
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return true;
    
    return OnGetStructProperty(connection, record->GetProperty(path));
}

bool HttpInterface::OnGetEntityProperty(pb::HttpConnection& connection, Uid recordId, Uid entityId, const std::string& path)
{
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return true;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return true;
    
    return OnGetStructProperty(connection, entity->GetProperty(path));
}

bool HttpInterface::OnGetStructProperty(pb::HttpConnection& connection, const Property* property)
{
    json::Object data;
    
    if (!property)
        return true;
    
    if (property->GetType() == Property::kPropertyAtom)
    {
        data["type"] = json::String("atom");
        data["value"] = json::String(static_cast<const PropertyAtom*>(property)->GetStringValue());
    }
    
    std::stringstream contentStream;
    json::Writer::Write(data, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnSetRecordProperty(pb::HttpConnection& connection, Uid recordId, const std::string& path, const std::string& type, const std::string& value)
{
    std::string propertyValue;
    
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    if (type == "atom")
    {
        PropertyAtom* property = record->AcquireAtom(path);
        
        if (!property)
            return false;
        
        property->SetStringValue(value);
    }
    
    return true;
}

bool HttpInterface::OnSetEntityProperty(pb::HttpConnection& connection, Uid recordId, Uid entityId, const std::string& path, const std::string& type, const std::string& value)
{
    std::string propertyValue;
    
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return false;
    
    if (type == "atom")
    {
        PropertyAtom* property = entity->AcquireAtom(path);
        
        if (!property)
            return false;
        
        property->SetStringValue(value);
    }
    
    return true;
}

bool HttpInterface::OnGetImage(pb::HttpConnection& connection, const std::string& image)
{
    Project* project = Core::Instance()->GetProject();

    const Project::ProjectConfig& config = project->GetConfig();
  
    std::fstream file;
    
    for (std::vector<std::string>::const_iterator it = config.imageRoots.begin(); it != config.imageRoots.end(); ++it)
    {
        file.open((*it + image).c_str());
        
        if (file.is_open())
            break;
    }
    
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        char contentLength[64];
        sprintf(contentLength, "%d", static_cast<int>(content.length()));
        connection.AddHeader("Content-Length", contentLength);
        connection.SetContent(content);
        
        return true;
    }
    
    return false;
}
