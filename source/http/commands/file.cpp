#include "command/manager.h"
#include "http/commands/file.h"
#include "http/httpInterface.h"
#include "core.h"

using namespace pixeleditor;

FileCommands::FileCommands()
{
    Core::Instance()->GetHttpInterface()->RegisterCommand("export", pb::HttpServer::kRequestTypePut, HttpInterface::CommandDelegate::from_method<FileCommands, &FileCommands::OnExport>(this));
    Core::Instance()->GetHttpInterface()->RegisterCommand("save", pb::HttpServer::kRequestTypePut, HttpInterface::CommandDelegate::from_method<FileCommands, &FileCommands::OnSave>(this));
}

bool FileCommands::OnExport(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments)
{
    Core::Instance()->GetCommandManager()->Exec("export");
    return true;
}

bool FileCommands::OnSave(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments)
{
    Core::Instance()->GetCommandManager()->Exec("save");
    return true;
}
