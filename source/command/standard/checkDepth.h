#pragma once

#include "command/base.h"
#include "project/definitions.h"

class CheckDepthCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
public:
    CheckDepthCommand();
    virtual ~CheckDepthCommand();
    
    virtual std::string GetName();
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString);
    
private:
};
