#pragma once

#include "Gwen/Controls/WindowControl.h"

namespace Gwen
{
    namespace Controls
    {
        class Button;
        class TextBox;
    }
}

namespace pixeleditor
{

class NetworkWindow : public Gwen::Controls::WindowControl
{
public:
    NetworkWindow(Gwen::Controls::Base* parent);
    ~NetworkWindow();
            
private:
    void OnAddressChanged(Gwen::Controls::Base* item);
    void OnConnect(Gwen::Controls::Base* item);
    
    Gwen::Controls::TextBox* _AddressBox;
    Gwen::Controls::TextBox* _PortBox;
    Gwen::Controls::Button* _ReconnectButton;
};
    
}
