#pragma once

#include "command/base.h"

namespace pixeleditor
{

class SaveCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do();
};
    
class ExportCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do();
};

class OpenCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do();
};

class CloseCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do();
};
    
}
