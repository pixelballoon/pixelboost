#include <sstream>

#include "pixelboost/data/json/writer.h"

#include "http/commands/record.h"
#include "http/httpInterface.h"
#include "project/project.h"
#include "project/record.h"
#include "core.h"

using namespace pixeleditor;

RecordCommands::RecordCommands()
{
    Core::Instance()->GetHttpInterface()->RegisterCommand("records", pixelboost::HttpServer::kRequestTypeGet, HttpInterface::CommandDelegate::from_method<RecordCommands, &RecordCommands::OnGetRecords>(this));
}

bool RecordCommands::OnGetRecords(pixelboost::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments)
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