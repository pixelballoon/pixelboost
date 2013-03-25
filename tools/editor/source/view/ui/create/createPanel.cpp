/*
#include "glm/glm.hpp"

#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/ImagePanel.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/Layout/Tile.h"

#include "project/project.h"
#include "view/manipulator/create.h"
#include "view/ui/create/createPanel.h"
#include "core.h"
#include "view.h"

static const int TILE_SIZE = 100;

class CreateButton : public Gwen::Controls::Button
{
public:
    CreateButton(Gwen::Controls::Base* parent, glm::vec2 size)
        : Gwen::Controls::Button(parent)
        , _Size(size)
    {
        SetSize(size.x, size.y);
    }
    
    void SetSprite(const std::string& sprite)
    {
        SetImage("editor_images/"+sprite, true);
        if (m_Image)
        {
            Gwen::Point newSize = Gwen::Point(_Size.x-10, _Size.y-10);
            Gwen::Point size = m_Image->GetSize();
            if (size.y > size.x)
            {
                m_Image->SetSize(size.x * ((float)newSize.y/(float)size.y), newSize.y);
            } else {
                m_Image->SetSize(newSize.x, size.y * ((float)newSize.x/(float)size.x));
            }
        }
    }
    
private:
    glm::vec2 _Size;
};

CreatePanel::CreatePanel(Gwen::Controls::Base* parent)
    : Gwen::Controls::ScrollControl(parent)
{
    _Tabs = new Gwen::Controls::TabControl(this);
    _Tabs->Dock(Gwen::Pos::Fill);

    Core::Instance()->GetProject()->projectOpened.Connect(this, &CreatePanel::OnProjectOpened);
    Core::Instance()->GetProject()->projectClosed.Connect(this, &CreatePanel::OnProjectClosed);
}

CreatePanel::~CreatePanel()
{
    
}

void CreatePanel::Clear()
{
    while (_Tabs->TabCount())
    {
        Gwen::Controls::TabButton* tab = _Tabs->GetTab(0);
        _Tabs->RemovePage(tab);
        tab->DelayedDelete();
    }
}

void CreatePanel::OnProjectOpened(Project* project)
{
    Clear();
    
    for (json::Object::const_iterator it = project->GetConfig().assets.Begin(); it != project->GetConfig().assets.End(); ++it)
    {
        Gwen::Controls::Layout::Tile* container = new Gwen::Controls::Layout::Tile(this);
        container->SetTileSize(TILE_SIZE, TILE_SIZE);
        _Tabs->AddPage(it->name, container);
        
        json::Object definition = it->element;
        
        std::string entity = (json::String)definition["entity"];
        std::string type = (json::String)definition["type"];
        std::vector<std::string> fields;
        
        json::Array definitionFields = definition["fields"];
        for (json::Array::const_iterator fieldIt = definitionFields.Begin(); fieldIt != definitionFields.End(); ++fieldIt)
        {
            fields.push_back((json::String)*fieldIt);
        }
        
        if (type == "defined")
        {
            json::Array data = definition["data"];
            
            for (json::Array::const_iterator dataIt = data.Begin(); dataIt != data.End(); ++dataIt)
            {
                std::vector<std::string> values;
                json::Array definitionValues = *dataIt;
                for (json::Array::const_iterator valueIt = definitionValues.Begin(); valueIt != definitionValues.End(); ++valueIt)
                {
                    values.push_back((json::String)*valueIt);
                }
                
                if (values.size() >= fields.size())
                {
                    CreateButton* button = new CreateButton(container, glm::vec2(TILE_SIZE-2,TILE_SIZE-2));
                    
                    std::string sprite = (values.size() > fields.size()) ? values[fields.size()] : values[0];
                    
                    button->SetSprite(sprite);
                    button->UserData.Set("entity", entity);
                    button->UserData.Set("fields", fields);
                    button->UserData.Set("values", values);
                    button->onPress.Add(this, &CreatePanel::OnItemSelected);
                }
            }
        }
    }
}

void CreatePanel::OnProjectClosed(Project* project)
{
    Clear();
}

void CreatePanel::OnItemSelected(Gwen::Controls::Base* item)
{
    CreateManipulator* createManipulator = static_cast<CreateManipulator*>(View::Instance()->GetManipulatorManager()->SetActiveManipulator("create"));
    
    createManipulator->SetEntityType(item->UserData.Get<std::string>("entity"));
    createManipulator->SetCreationData(item->UserData.Get<std::vector<std::string> >("fields"), item->UserData.Get<std::vector<std::string> >("values"));
}
*/
