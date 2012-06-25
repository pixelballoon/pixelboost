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
    
    class MoveManipulatorWindow : public Gwen::Controls::WindowControl
    {
    public:
        MoveManipulatorWindow(Gwen::Controls::Base* parent);
        ~MoveManipulatorWindow();
        
    private:
        void OnSnapChanged(Gwen::Controls::Base* item);
        
        Gwen::Controls::TextBox* _SnapX;
        Gwen::Controls::TextBox* _SnapY;
        Gwen::Controls::TextBox* _SnapZ;
    };
    
}
