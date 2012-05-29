#pragma once

#include "Gwen/Controls/WindowControl.h"

namespace pixeleditor
{

class NetworkWindow : public Gwen::Controls::WindowControl
{
public:
    NetworkWindow(Gwen::Controls::Base* parent);
    ~NetworkWindow();
            
private:
    void OnAddressChanged(Gwen::Controls::Base* item);
};
    
}
