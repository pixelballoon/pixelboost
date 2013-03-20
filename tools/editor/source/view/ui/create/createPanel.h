#pragma once

#include "Gwen/Controls/ScrollControl.h"

namespace Gwen
{
    namespace Controls
    {
        class TabControl;
        
        namespace Layout 
        {
            class Tile;
        }
    }
}

class Project;

class CreatePanel : public Gwen::Controls::ScrollControl
{
public:
    CreatePanel(Gwen::Controls::Base* parent);
    ~CreatePanel();
    
private:
    void Clear();
    
    void OnProjectOpened(Project* project);
    void OnProjectClosed(Project* project);
    
    void OnItemSelected(Gwen::Controls::Base* item);
    
private:
    Gwen::Controls::TabControl* _Tabs;
};    
