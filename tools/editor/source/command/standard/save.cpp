#include <string>

#include "command/standard/file.h"
#include "project/project.h"
#include "core.h"

Command* SaveCommand::Create()
{
    return new SaveCommand();
}

std::string SaveCommand::GetStaticName()
{
    return "save";
}

std::string SaveCommand::GetName()
{
    return SaveCommand::GetStaticName();
}

bool SaveCommand::Do(std::string& returnString)
{
    return Core::Instance()->GetProject()->Save();
}
