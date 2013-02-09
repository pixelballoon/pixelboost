#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/ComboBox.h"
#include "Gwen/Controls/GroupBox.h"
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

static const int ATOM_HEIGHT = 25;
static const int ATOM_PADDING = 5;
static const int ATOM_WIDTH = 200;
static const int POINTER_HEIGHT = 25;
static const int POINTER_PADDING = 5;
static const int POINTER_WIDTH = 200;
static const int REFERENCE_HEIGHT = 25;
static const int REFERENCE_PADDING = 5;
static const int REFERENCE_WIDTH = 200;
static const int STRUCT_INDENT = 20;
static const int STRUCT_PADDING_INNER = 10;
static const int STRUCT_PADDING_OUTER = 20;
static const int STRUCT_WIDTH = 250;

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
    
    if (item && item->GetType())
    {
        AddStruct(glm::vec2(0,0), item->GetType(), item, "/");
    }
}

glm::vec2 PropertyPanel::AddStruct(glm::vec2 offset, const SchemaStruct* schemaStruct, ProjectStruct* item, const std::string& path)
{
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->first + "/";
        
        Gwen::Controls::GroupBox* surround = 0;
        glm::vec2 previousOffset = offset;
        
        if (it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyStruct)
        {
            surround = new Gwen::Controls::GroupBox(this);
            surround->SetPos(previousOffset.x, previousOffset.y);
            surround->SetText(it->second->GetName());
        } else {
            Gwen::Controls::Label* label = new Gwen::Controls::Label(this);
            label->SetText(it->second->GetName());
            label->SetPos(previousOffset.x, previousOffset.y);
            label->SetSize(label->GetSize().x, 20);
        }
        
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
                offset = AddPointer(offset, static_cast<SchemaPropertyPointer*>(it->second), item, propertyPath);
                break;
            }
                
            case SchemaProperty::kSchemaPropertyReference:
            {
                offset = AddReference(offset, static_cast<SchemaPropertyReference*>(it->second), item, propertyPath);
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
        
        if (surround)
        {
            surround->SetSize(STRUCT_WIDTH, offset.y-previousOffset.y + STRUCT_PADDING_INNER);
            offset.y += STRUCT_PADDING_OUTER;
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

glm::vec2 PropertyPanel::AddPointer(glm::vec2 offset, SchemaPropertyPointer* pointer, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyPointer* propertyPointer = property ? property->AsPointer() : 0;
    
    Gwen::Controls::ComboBox* comboBox = new Gwen::Controls::ComboBox(this);
    comboBox->SetPos(offset.x, offset.y);
    comboBox->SetSize(POINTER_WIDTH, POINTER_HEIGHT);
    comboBox->UserData.Set("struct", item);
    comboBox->UserData.Set("path", path);
    comboBox->AddItem(Gwen::Utility::StringToUnicode("<No Entity>"))->UserData.Set<pb::Uid>("uid", 0);
    comboBox->onSelection.Add(this, &PropertyPanel::OnPointerChanged);
    
    ProjectRecord* projectRecord = item->GetRecord();
    const SchemaEntity* pointerType = pointer->GetSchemaEntity();

    if (pointerType && projectRecord)
    {
        const ProjectRecord::EntityMap& entities = projectRecord->GetEntities();
        
        for (ProjectRecord::EntityMap::const_iterator it = entities.begin(); it != entities.end(); ++it)
        {
            if (it->second->GetType() == pointerType)
            {
                Gwen::Controls::MenuItem* menuItem = comboBox->AddItem(Gwen::Utility::StringToUnicode(it->second->GetName()));
                menuItem->UserData.Set("uid", it->second->GetUid());
                
                if (propertyPointer && it->second->GetUid() == propertyPointer->GetPointerValue())
                {
                    comboBox->SelectItem(menuItem);
                }
            }
        }
    }
    
    return offset + glm::vec2(0, POINTER_HEIGHT + POINTER_PADDING);
}

glm::vec2 PropertyPanel::AddReference(glm::vec2 offset, SchemaPropertyReference* reference, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyReference* propertyReference = property ? property->AsReference() : 0;
    
    Gwen::Controls::ComboBox* comboBox = new Gwen::Controls::ComboBox(this);
    comboBox->SetPos(offset.x, offset.y);
    comboBox->SetSize(REFERENCE_WIDTH, REFERENCE_HEIGHT);
    comboBox->UserData.Set("struct", item);
    comboBox->UserData.Set("path", path);
    comboBox->AddItem(Gwen::Utility::StringToUnicode("<No Record>"))->UserData.Set<pb::Uid>("uid", 0);
    comboBox->onSelection.Add(this, &PropertyPanel::OnReferenceChanged);
    
    Project* project = item->GetProject();
    const SchemaRecord* referenceType = reference->GetSchemaRecord();
    
    if (referenceType && project)
    {
        const Project::RecordMap& records = project->GetRecords();
        
        for (Project::RecordMap::const_iterator it = records.begin(); it != records.end(); ++it)
        {
            if (it->second->GetType() == referenceType)
            {
                Gwen::Controls::MenuItem* menuItem = comboBox->AddItem(Gwen::Utility::StringToUnicode(it->second->GetName()));
                menuItem->UserData.Set("uid", it->second->GetUid());
                
                if (propertyReference && it->second->GetUid() == propertyReference->GetReferenceValue())
                {
                    comboBox->SelectItem(menuItem);
                }
            }
        }
    }
    
    return offset + glm::vec2(0, REFERENCE_HEIGHT + REFERENCE_PADDING);
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

void PropertyPanel::OnPointerChanged(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");
    
    Gwen::Controls::ComboBox* comboBox = static_cast<Gwen::Controls::ComboBox*>(input);
    
    Gwen::Controls::Label* selectedItem = comboBox->GetSelectedItem();
    
    PropertyPointer* pointer = structItem->AcquirePointer(path);
    
    if (selectedItem)
    {
        pb::Uid uid = selectedItem->UserData.Get<pb::Uid>("uid");
        pointer->SetPointerValue(uid);
    }
}

void PropertyPanel::OnReferenceChanged(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");
    
    Gwen::Controls::ComboBox* comboBox = static_cast<Gwen::Controls::ComboBox*>(input);
    
    Gwen::Controls::Label* selectedItem = comboBox->GetSelectedItem();
    
    PropertyReference* reference = structItem->AcquireReference(path);
    
    if (selectedItem)
    {
        pb::Uid uid = selectedItem->UserData.Get<pb::Uid>("uid");
        reference->SetReferenceValue(uid);
    }
}
