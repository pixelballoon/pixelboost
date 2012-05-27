#pragma once

#include "command/base.h"
#include "project/definitions.h"

namespace pixeleditor
{

class CreateEntityCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
public:
    CreateEntityCommand();
    virtual ~CreateEntityCommand();
    
    virtual std::string GetName();
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString);
    virtual bool Undo();
    
private:
    Uid _RecordId;
    Uid _EntityUid;
};
    
}
