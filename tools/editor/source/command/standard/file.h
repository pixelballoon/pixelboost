#pragma once

#include "command/base.h"

class SaveCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do(std::string& returnString);
};
    
class ExportCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do(std::string& returnString);
};

class OpenCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do(std::string& returnString);
};

class CloseCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    virtual std::string GetName();
    
    virtual bool Do(std::string& returnString);
};
