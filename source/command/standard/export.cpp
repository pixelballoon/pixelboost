#include <string>

#include "command/standard/file.h"
#include "project/project.h"
#include "core.h"

using namespace pixeleditor;
    
Command* ExportCommand::Create()
{
    return new ExportCommand();
}

std::string ExportCommand::GetStaticName()
{
    return "export";
}

std::string ExportCommand::GetName()
{
    return ExportCommand::GetStaticName();
}

bool ExportCommand::Do(std::string& returnString)
{
    char cmd[2048];
    std::string outputDir = Core::Instance()->GetProject()->GetConfig().exportDir + "templates/";
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);
    
    return Core::Instance()->GetProject()->Export();
}
