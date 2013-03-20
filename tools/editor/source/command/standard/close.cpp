#include <string>

#include "command/standard/file.h"
#include "project/project.h"
#include "core.h"

Command* CloseCommand::Create()
{
    return new CloseCommand();
}

std::string CloseCommand::GetStaticName()
{
    return "close";
}

std::string CloseCommand::GetName()
{
    return CloseCommand::GetStaticName();
}

bool CloseCommand::Do(std::string& returnString)
{
    return Core::Instance()->GetProject()->Close();
}
