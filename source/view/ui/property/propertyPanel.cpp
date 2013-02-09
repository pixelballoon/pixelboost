#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/TextBox.h"

#include "core/selection.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/ui/property/propertyPanel.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

static const int ATOM_PADDING = 5;
static const int ATOM_WIDTH = 200;
static const int ATOM_HEIGHT = 25;
static const int STRUCT_INDENT = 20;

PropertyPanel::PropertyPanel(Gwen::Controls::Base* parent)
    : Gwen::Controls::ScrollControl(parent)
    , _Pinned(false)
{
    Core::Instance()->GetSelection().selectionChanged.Connect(this, &PropertyPanel::OnSelectionChanged);
}

PropertyPanel::~PropertyPanel()
{
    
}

void PropertyPanel::SetPinned(bool pinned)
{
    _Pinned = pinned;
}

void PropertyPanel::SetStruct(ProjectStruct* item, const std::string& focusPath)
{
    Clear();
    
    if (item)
    {
        AddStruct(glm::vec2(0,0), item->GetType(), item, "/");
    }
}

glm::vec2 PropertyPanel::AddStruct(glm::vec2 offset, const SchemaStruct* schemaStruct, ProjectStruct* item, const std::string& path)
{
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->first + "/";
        
        Gwen::Controls::Label* label = new Gwen::Controls::Label(this);
        label->SetText(it->second->GetName());
        label->SetPos(offset.x, offset.y);
        label->SetSize(label->GetSize().x, 20);
        
        offset += glm::vec2(0,20);
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                offset = AddAtom(offset, static_cast<SchemaPropertyAtom*>(it->second), item, propertyPath);
                break;
            }
                
            case SchemaProperty::kSchemaPropertyPointer:
            {
                break;
            }
                
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* schemaPropertyStruct = static_cast<SchemaPropertyStruct*>(it->second);
                
                offset = AddStruct(offset + glm::vec2(STRUCT_INDENT,0), schemaPropertyStruct->GetSchemaStruct(), item, propertyPath) - glm::vec2(STRUCT_INDENT,0);
                break;
            }
            
            case SchemaProperty::kSchemaPropertyArray:
            {
                break;
            }
        }
    }
    
    return offset;
}

glm::vec2 PropertyPanel::AddAtom(glm::vec2 offset, SchemaPropertyAtom* atom, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyAtom* propertyAtom = property ? property->AsAtom() : 0;
    
    Gwen::Controls::TextBox* textBox = new Gwen::Controls::TextBox(this);
    textBox->SetText(propertyAtom ? propertyAtom->GetStringValue() : "");
    textBox->SetPos(offset.x, offset.y);
    textBox->SetSize(ATOM_WIDTH, ATOM_HEIGHT);
    textBox->UserData.Set("struct", item);
    textBox->UserData.Set("path", path);
    textBox->onTextChanged.Add(this, &PropertyPanel::OnAtomChanged);
    
    return offset + glm::vec2(0, ATOM_HEIGHT + ATOM_PADDING);
}

void PropertyPanel::OnSelectionChanged(const pixeleditor::Selection* selection)
{
    if (!_Pinned)
    {
        Selection::Entities::const_reverse_iterator entityIt = selection->GetSelection().rbegin();
        
        if (entityIt == selection->GetSelection().rend())
        {
            SetStruct(View::Instance()->GetRecord());
            return;
        }
        
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(entityIt->first);
        
        if (entity)
            SetStruct(entity->GetEntity(), "");
    }
}

void PropertyPanel::OnAtomChanged(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");

    PropertyAtom* atom = structItem->AcquireAtom(path);
    atom->SetStringValue(input->GetValue());
}
