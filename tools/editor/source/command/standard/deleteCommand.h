#pragma once

#include <utility>

#include "command/base.h"
#include "project/definitions.h"

class ProjectEntity;

class DeleteCommand : public Command
{
public:
    static Command* Create();
    static std::string GetStaticName();
    
public:
    DeleteCommand();
    virtual ~DeleteCommand();
    
    virtual std::string GetName();
    
    virtual bool CanUndo();
    virtual bool Do(std::string& returnString);
    virtual bool Undo();
    
private:
    std::vector<std::pair<Uid, ProjectEntity*> > _Entities;
};
