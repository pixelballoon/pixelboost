#pragma once

#include "command/base.h"
#include "project/definitions.h"

namespace pixeleditor
{
    
    class CopyCommand : public Command
    {
    public:
        static Command* Create();
        static std::string GetStaticName();
        
    public:
        CopyCommand();
        virtual ~CopyCommand();
        
        virtual std::string GetName();
        
        virtual bool CanUndo();
        virtual bool Do(std::string& returnString);
        
    private:
    };
    
}
