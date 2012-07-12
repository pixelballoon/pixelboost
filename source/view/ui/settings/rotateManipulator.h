#pragma once

#include "Gwen/Controls/WindowControl.h"

namespace Gwen
{
    namespace Controls
    {
        class TextBox;
    }
}

namespace pixeleditor
{
    
    class RotateManipulatorWindow : public Gwen::Controls::WindowControl
    {
    public:
        RotateManipulatorWindow(Gwen::Controls::Base* parent);
        ~RotateManipulatorWindow();
        
    private:
        void OnSnapChanged(Gwen::Controls::Base* item);
        
        Gwen::Controls::TextBox* _SnapX;
        Gwen::Controls::TextBox* _SnapY;
        Gwen::Controls::TextBox* _SnapZ;
    };
    
}
