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

static const int ARRAY_BUTTON_HEIGHT = 25;
static const int ARRAY_BUTTON_SPACING = 5;
static const int ARRAY_BUTTON_WIDTH = 25;
static const int ARRAY_ITEM_INDENT = 20;
static const int ARRAY_PADDING = 5;
static const int ATOM_HEIGHT = 25;
static const int ATOM_PADDING = 5;
static const int ATOM_WIDTH = 200;
static const int GROUP_INDENT = 20;
static const int GROUP_PADDING_INNER = 10;
static const int GROUP_PADDING_OUTER = 20;
static const int LABEL_HEIGHT = 20;
static const int POINTER_HEIGHT = 25;
static const int POINTER_PADDING = 5;
static const int POINTER_WIDTH = 200;
static const int REFERENCE_HEIGHT = 25;
static const int REFERENCE_PADDING = 5;
static const int REFERENCE_WIDTH = 200;

PropertyPanel::PropertyPanel(Gwen::Controls::Base* parent)
    : Gwen::Controls::ScrollControl(parent)
    , _Pinned(false)
    , _SelectedItem(0)
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
    
    if (_SelectedItem)
    {
        _SelectedItem->propertyChanged.Disconnect(this, &PropertyPanel::OnStructChanged);
        _SelectedItem = 0;
    }
    
    _SelectedItem = item;
    
    if (_SelectedItem && _SelectedItem->GetType())
    {
        AddStruct(glm::vec2(0,0), _SelectedItem->GetType(), _SelectedItem, "/");
        
        _SelectedItem->propertyChanged.Connect(this, &PropertyPanel::OnStructChanged);
    }
}

void PropertyPanel::OnStructChanged(ProjectStruct* structure, Property* property)
{
    if (property->GetType() == Property::kPropertyArray)
    {
        Clear();
        
        AddStruct(glm::vec2(0,0), _SelectedItem->GetType(), _SelectedItem, "/");
    }
}

glm::vec2 PropertyPanel::AddProperty(glm::vec2 offset, const SchemaProperty* property, ProjectStruct* item, const std::string& path)
{
    switch (property->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            return AddAtom(offset, static_cast<const SchemaPropertyAtom*>(property), item, path);
        }
            
        case SchemaProperty::kSchemaPropertyPointer:
        {
            return AddPointer(offset, static_cast<const SchemaPropertyPointer*>(property), item, path);
        }
            
        case SchemaProperty::kSchemaPropertyReference:
        {
            return AddReference(offset, static_cast<const SchemaPropertyReference*>(property), item, path);
        }
            
        case SchemaProperty::kSchemaPropertyStruct:
        {
            const SchemaPropertyStruct* schemaPropertyStruct = static_cast<const SchemaPropertyStruct*>(property);
            
            return AddStruct(offset, schemaPropertyStruct->GetSchemaStruct(), item, path);
        }
            
        case SchemaProperty::kSchemaPropertyArray:
        {
            return AddArray(offset, static_cast<const SchemaPropertyArray*>(property), item, path);
        }
    }
    
    return glm::vec2(0,0);
}

glm::vec2 PropertyPanel::AddStruct(glm::vec2 offset, const SchemaStruct* schemaStruct, ProjectStruct* item, const std::string& path)
{
    glm::vec2 size;
    
	if (!schemaStruct)
	{
		return size;
	}
	
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->first + "/";
        
        Gwen::Controls::GroupBox* surround = 0;
        glm::vec2 previousOffset = offset;
        
        if (it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyStruct || it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyArray)
        {
            surround = new Gwen::Controls::GroupBox(this);
            surround->SetPos(previousOffset.x, previousOffset.y);
            surround->SetText(it->second->GetName());
        } else {
            Gwen::Controls::Label* label = new Gwen::Controls::Label(this);
            label->SetText(it->second->GetName());
            label->SetPos(previousOffset.x, previousOffset.y);
            label->SetSize(label->GetSize().x, LABEL_HEIGHT);
        }
        
        glm::vec2 propertySize = AddProperty(offset + glm::vec2(surround ? GROUP_INDENT : 0,LABEL_HEIGHT), it->second, item, propertyPath) + glm::vec2(0,LABEL_HEIGHT);
        size = glm::vec2(glm::max(size.x, propertySize.x), size.y + propertySize.y);
        offset.y += propertySize.y;
        
        if (surround)
        {
            size += glm::vec2(GROUP_PADDING_INNER*2.f + GROUP_INDENT, GROUP_PADDING_INNER);
            surround->SetSize(propertySize.x + GROUP_PADDING_INNER*2.f + GROUP_INDENT, propertySize.y + GROUP_PADDING_INNER);
            offset.y += GROUP_PADDING_OUTER;
            size.y += GROUP_PADDING_OUTER;
        }
    }
	
    glm::vec2 structSize = AddStruct(offset, schemaStruct->GetBaseType(), item, path);
	size = glm::vec2(glm::max(size.x, structSize.x), size.y + structSize.y);
    
    return size;
}

glm::vec2 PropertyPanel::AddAtom(glm::vec2 offset, const SchemaPropertyAtom* atom, ProjectStruct* item, const std::string& path)
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
    
    return glm::vec2(ATOM_WIDTH, ATOM_HEIGHT + ATOM_PADDING);
}

glm::vec2 PropertyPanel::AddPointer(glm::vec2 offset, const SchemaPropertyPointer* pointer, ProjectStruct* item, const std::string& path)
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
                    comboBox->SelectItem(menuItem, false);
                }
            }
        }
    }
    
    return glm::vec2(POINTER_WIDTH, POINTER_HEIGHT + POINTER_PADDING);
}

glm::vec2 PropertyPanel::AddReference(glm::vec2 offset, const SchemaPropertyReference* reference, ProjectStruct* item, const std::string& path)
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
    
    return glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT + REFERENCE_PADDING);
}

glm::vec2 PropertyPanel::AddArray(glm::vec2 offset, const SchemaPropertyArray* array, ProjectStruct* item, const std::string& path)
{
    glm::vec2 size;
    
    const Property* property = item->GetProperty(path);
    if (property)
    {
        const PropertyArray* propertyArray = property->AsArray();
        
        for (int i=0; i<propertyArray->GetElementCount(); ++i)
        {
            glm::vec2 startOffset = offset;
            
            char labelText[16];
            snprintf(labelText, 16, "%d:", i+1);
            
            Gwen::Controls::Label* itemLabel = new Gwen::Controls::Label(this);
            itemLabel->SetPos(startOffset.x, startOffset.y);
            itemLabel->SetSize(ARRAY_ITEM_INDENT, ARRAY_BUTTON_HEIGHT);
            itemLabel->SetText(labelText);
            
            char itemPath[1024];
            snprintf(itemPath, 1024, "%s#%X/", path.c_str(), propertyArray->GetElementIdByIndex(i));
            glm::vec2 propertySize = AddProperty(offset + glm::vec2(ARRAY_ITEM_INDENT, 0), array->GetSchemaProperty(), item, itemPath) + glm::vec2(ARRAY_ITEM_INDENT, 0);
            offset.y += propertySize.y;
            size = glm::vec2(glm::max(size.x, propertySize.x + ARRAY_ITEM_INDENT), size.y+propertySize.y);
            
            Gwen::Controls::Button* insertItemButton = new Gwen::Controls::Button(this);
            insertItemButton->SetText("+");
            insertItemButton->SetPos(startOffset.x + propertySize.x + ARRAY_BUTTON_SPACING, startOffset.y);
            insertItemButton->SetSize(ARRAY_BUTTON_WIDTH, ARRAY_BUTTON_HEIGHT);
            insertItemButton->UserData.Set("struct", item);
            insertItemButton->UserData.Set("path", path);
            insertItemButton->UserData.Set("index", i);
            insertItemButton->onPress.Add(this, &PropertyPanel::OnInsertArrayItem);
            
            Gwen::Controls::Button* removeItemButton = new Gwen::Controls::Button(this);
            removeItemButton->SetText("-");
            removeItemButton->SetPos(startOffset.x + propertySize.x + ARRAY_BUTTON_SPACING + ARRAY_BUTTON_WIDTH + ARRAY_BUTTON_SPACING, startOffset.y);
            removeItemButton->SetSize(ARRAY_BUTTON_WIDTH, ARRAY_BUTTON_HEIGHT);
            removeItemButton->UserData.Set("struct", item);
            removeItemButton->UserData.Set("path", path);
            removeItemButton->UserData.Set("index", i);
            removeItemButton->onPress.Add(this, &PropertyPanel::OnRemoveArrayItem);
        }
    }
    
    Gwen::Controls::Button* addItemButton = new Gwen::Controls::Button(this);
    addItemButton->SetText("+");
    addItemButton->SetPos(offset.x + ARRAY_ITEM_INDENT, offset.y);
    addItemButton->SetSize(ARRAY_BUTTON_WIDTH, ARRAY_BUTTON_HEIGHT);
    addItemButton->UserData.Set("struct", item);
    addItemButton->UserData.Set("path", path);
    addItemButton->onPress.Add(this, &PropertyPanel::OnAppendArrayItem);

    size += glm::vec2(ARRAY_BUTTON_WIDTH*2 + ARRAY_BUTTON_SPACING*2, ARRAY_BUTTON_HEIGHT + ARRAY_PADDING);
    
    return size;
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

void PropertyPanel::OnAppendArrayItem(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");
    
    PropertyArray* array = structItem->AcquireArray(path);
    
    array->AddElement();
}

void PropertyPanel::OnRemoveArrayItem(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");
    unsigned int index = input->UserData.Get<unsigned int>("index");
    
    PropertyArray* array = structItem->AcquireArray(path);
    
    array->RemoveElementByIndex(index);
}

void PropertyPanel::OnInsertArrayItem(Gwen::Controls::Base* input)
{
    ProjectStruct* structItem = input->UserData.Get<ProjectStruct*>("struct");
    std::string path = input->UserData.Get<std::string>("path");
    unsigned int index = input->UserData.Get<unsigned int>("index");
    
    PropertyArray* array = structItem->AcquireArray(path);
    
    array->AddElementBeforeIndex(index);
}
