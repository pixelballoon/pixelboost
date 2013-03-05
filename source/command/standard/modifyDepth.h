#pragma once

#include "command/base.h"
#include "project/definitions.h"

class ModifyDepthCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
public:
    ModifyDepthCommand();
    virtual ~ModifyDepthCommand();
    
    virtual std::string GetName();
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString);
    
private:
};
