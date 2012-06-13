#pragma once

#include "command/base.h"
#include "core/selection.h"
#include "project/definitions.h"

namespace pixeleditor
{
    
    class PasteCommand : public Command
    {
    public:
        static Command* Create();
        static std::string GetStaticName();
        
    public:
        PasteCommand();
        virtual ~PasteCommand();
        
        virtual std::string GetName();
        
        virtual bool CanUndo();
        virtual bool Do(std::string& returnString);
        virtual bool Undo();
        
    private:
        Selection _Selection;
    };
    
}
