#pragma once

#include "glm/glm.hpp"

#include "command/base.h"

namespace pixeleditor
{
    class TransformCommand : public Command
    {
    public:
        static Command* Create();
        static std::string GetStaticName();
        
    public:
        TransformCommand();
        virtual ~TransformCommand();
        
        virtual std::string GetName();
        
        virtual bool CanUndo();
        virtual bool Do(std::string& returnString);
        virtual bool Undo();
        
    private:
        bool _TransformPosition;
        bool _TransformRotation;
        bool _TransformScale;
        
        glm::vec3 _PositionOffset;
        float _RotationOffset;
        glm::vec3 _ScaleOffset;
    };    
}
