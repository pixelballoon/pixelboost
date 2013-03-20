#pragma once

#include <map>
#include <string>
#include <vector>

class Command;

class Command
{
public:
    virtual ~Command();
    
    bool IsArgumentSet(const std::string& argument="");
    unsigned long GetArgumentCount(const std::string& argument="");
    std::string GetArgument(const std::string& argument="", unsigned long index=0);
    void SetArguments(const std::string& arguments);
    
    virtual std::string GetName() = 0;
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString) = 0;
    virtual bool Undo();
    
private:
    typedef std::map<std::string, std::vector<std::string> > ArgumentMap;
    ArgumentMap _Arguments;
};
