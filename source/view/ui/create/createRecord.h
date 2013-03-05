#pragma once

#include "Gwen/Controls/WindowControl.h"

namespace Gwen
{
    namespace Controls
    {
        class Button;
        class ComboBox;
        class TextBox;
    }
}

class CreateRecordWindow : public Gwen::Controls::WindowControl
{
public:
    CreateRecordWindow(Gwen::Controls::Base* parent);
    ~CreateRecordWindow();
    
private:
    void OnCreate(Gwen::Controls::Base* item);
    
    Gwen::Controls::TextBox* _NameField;
    Gwen::Controls::ComboBox* _TypeField;
    Gwen::Controls::Button* _CreateButton;
};
