#pragma once

#include "command/base.h"
#include "core/selection.h"

class SelectCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
    SelectCommand();
    virtual ~SelectCommand();
    
    virtual std::string GetName();
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString);
    virtual bool Undo();
    
private:
    Selection _Selection;
};
