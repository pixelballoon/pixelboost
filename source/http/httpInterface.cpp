#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "pixelboost/data/json/writer.h"
#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/network/networkMessage.h"

#include "http/httpInterface.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "core.h"

using namespace pixelboost;
using namespace pixeleditor;

HttpInterface::HttpInterface(const std::string& htmlLocation)
    : pixelboost::HttpServer()
{
    Start(9090, pixelboost::FileHelpers::GetRootPath()+"/"+htmlLocation);
}

HttpInterface::~HttpInterface()
{
    
}

bool HttpInterface::OnHttpRequest(HttpServer::RequestType type, const std::string& uri, const std::string& query, const std::string& data, pixelboost::HttpConnection& connection)
{
    ssize_t split = uri.find('/', 1);
    
    std::string command;
    std::string argumentString;
    std::vector<std::string> arguments;
    
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
            arguments.push_back(argument);
        else
            break;
    }
    
    connection.AddHeader("Content-Type", "application/json;charset=utf-8");
    
    bool replied = false;
    
    if (command == "records" && type == kRequestTypeGet)
    {
        replied = OnGetRecords(connection);
    } else if (command == "record" && type == kRequestTypeGet)
    {
        if (arguments.size() == 2 && arguments[1] == "entities")
        {
            std::string record = arguments[0];
            replied = OnGetEntities(connection, atoi(record.c_str()));
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

bool HttpInterface::OnGetRecords(pixelboost::HttpConnection& connection)
{
    json::Array records;
    
    Project* project = Core::Instance()->GetProject();
    for (Project::RecordMap::const_iterator it = project->GetRecords().begin(); it != project->GetRecords().end(); ++it)
    {
        json::Object record;
        record["id"] = json::Number(it->first);
        record["name"] = json::String(it->second->GetName());
        record["type"] = json::String(it->second->GetTypeName());
        records.Insert(record);
    }
    
    std::stringstream contentStream;
    json::Writer::Write(records, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetEntities(pixelboost::HttpConnection& connection, Uid recordId)
{
    json::Array records;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    for (Record::EntityMap::const_iterator it = record->GetEntities().begin(); it != record->GetEntities().end(); ++it)
    {
        json::Object record;
        record["name"] = json::String(it->second->GetName());
        
        json::Object data;
        it->second->Export(data);
        record["data"] = data;
        
        records.Insert(record);
    }
    
    std::stringstream contentStream;
    json::Writer::Write(records, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}
