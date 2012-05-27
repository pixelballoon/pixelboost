#pragma once

#include "pixelboost/math/maths.h"

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
        
        Vec3 _PositionOffset;
        float _RotationOffset;
        Vec3 _ScaleOffset;
    };    
}
