#include <iostream>
#include <string>

#include "command/standard/file.h"
#include "project/project.h"
#include "core.h"

Command* OpenCommand::Create()
{
    return new OpenCommand();
}

std::string OpenCommand::GetStaticName()
{
    return "open";
}

std::string OpenCommand::GetName()
{
    return OpenCommand::GetStaticName();
}

bool OpenCommand::Do(std::string& returnString)
{
    return Core::Instance()->GetProject()->Open(GetArgument());
}
