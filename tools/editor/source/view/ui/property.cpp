#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/logic/message/graphics/gui.h"

#include "core/selection.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/ui/property.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

void PropertyUi::DoProperties(const pb::GuiRenderMessage& guiRenderMessage, ProjectStruct* item, const std::string& focusPath)
{
    if (item)
    {
        DoStruct(guiRenderMessage, item->GetType(), item, "/");
    }
}

void PropertyUi::DoStruct(const pb::GuiRenderMessage& guiRenderMessage, const SchemaStruct* schemaStruct, ProjectStruct* item, const std::string& path)
{
    if (!schemaStruct)
	{
		return;
	}
	
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string propertyPath = path + it->first + "/";
        auto pathHash = pb::TypeHash(propertyPath.c_str());
        
        pb::GuiId areaId;
        if (it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyStruct || it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyArray)
        {
            areaId = pb::GuiControls::BeginArea(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash));
            pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), it->second->GetName());
        } else {
            pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), it->second->GetName());
        }
        
        DoProperty(guiRenderMessage, it->second, item, propertyPath);
        
        if (areaId)
        {
            pb::GuiControls::EndArea(guiRenderMessage, areaId);
        }
    }
	
    DoStruct(guiRenderMessage, schemaStruct->GetBaseType(), item, path);
}

void PropertyUi::DoProperty(const pb::GuiRenderMessage& guiRenderMessage, const SchemaProperty* property, ProjectStruct* item, const std::string& path)
{
    switch (property->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            DoAtom(guiRenderMessage, static_cast<const SchemaPropertyAtom*>(property), item, path);
            break;
        }
            
        case SchemaProperty::kSchemaPropertyPointer:
        {
            DoPointer(guiRenderMessage, static_cast<const SchemaPropertyPointer*>(property), item, path);
            break;
        }
            
        case SchemaProperty::kSchemaPropertyReference:
        {
            DoReference(guiRenderMessage, static_cast<const SchemaPropertyReference*>(property), item, path);
            break;
        }
            
        case SchemaProperty::kSchemaPropertyStruct:
        {
            const SchemaPropertyStruct* schemaPropertyStruct = static_cast<const SchemaPropertyStruct*>(property);
            DoStruct(guiRenderMessage, schemaPropertyStruct->GetSchemaStruct(), item, path);
            break;
        }
            
        case SchemaProperty::kSchemaPropertyArray:
        {
            DoArray(guiRenderMessage, static_cast<const SchemaPropertyArray*>(property), item, path);
            break;
        }
    }
}

void PropertyUi::DoArray(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyArray* array, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    if (property)
    {
        const PropertyArray* propertyArray = property->AsArray();
        
        for (int i=0; i<propertyArray->GetElementCount(); ++i)
        {
            char labelText[16];
            snprintf(labelText, 16, "%d:", i+1);
            
            char itemPath[1024];
            snprintf(itemPath, 1024, "%s#%X/", path.c_str(), propertyArray->GetElementIdByIndex(i));
            
            auto pathHash = pb::TypeHash(itemPath);
            pb::GuiControls::BeginHorizontal(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), {pb::GuiLayoutHint::ExpandWidth()});
            pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), labelText, {pb::GuiLayoutHint::Width(50)});
            pb::GuiControls::BeginVertical(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), {pb::GuiLayoutHint::ExpandWidth()});
            DoProperty(guiRenderMessage, array->GetSchemaProperty(), item, itemPath);
            pb::GuiControls::EndVertical(guiRenderMessage);
            
            if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), "+", {pb::GuiLayoutHint::Width(20)}))
            {
                PropertyArray* array = item->AcquireArray(path);
                array->AddElementBeforeIndex(i);
            }
            
            if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, item + pathHash), "-", {pb::GuiLayoutHint::Width(20)}))
            {
                PropertyArray* array = item->AcquireArray(path);
                array->RemoveElementByIndex(i);
            }
            
            pb::GuiControls::EndHorizontal(guiRenderMessage);
        }
    }
    
    if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "+", {pb::GuiLayoutHint::Width(20)}))
    {
        PropertyArray* array = item->AcquireArray(path);
        array->AddElement();
    }
}

void PropertyUi::DoAtom(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyAtom* atom, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyAtom* propertyAtom = property ? property->AsAtom() : 0;
    
    auto textBox = pb::GuiControls::DoTextBox(guiRenderMessage, PbGuiId(guiRenderMessage, item + pb::TypeHash(path.c_str())), propertyAtom ? propertyAtom->GetStringValue() : "");
    if (textBox.first)
    {
        item->AcquireAtom(path)->SetStringValue(textBox.second);
    }
}

void PropertyUi::DoPointer(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyPointer* pointer, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyPointer* propertyPointer = property ? property->AsPointer() : 0;
    
    std::vector<ProjectEntity*> entityOptions;
    std::string summary = "<No Entity>";
    
    ProjectRecord* projectRecord = item->GetRecord();
    const SchemaEntity* pointerType = pointer->GetSchemaEntity();
    
    if (pointerType && projectRecord)
    {
        const ProjectRecord::EntityMap& entities = projectRecord->GetEntities();
        
        for (ProjectRecord::EntityMap::const_iterator it = entities.begin(); it != entities.end(); ++it)
        {
            if (it->second->GetType() == pointerType)
            {
                entityOptions.push_back(it->second);
                
                if (propertyPointer && it->second->GetUid() == propertyPointer->GetPointerValue())
                {
                    summary = it->second->GetName();
                }
            }
        }
    }
    
    std::sort(entityOptions.begin(), entityOptions.end(), [](ProjectEntity* a, ProjectEntity* b) { return a->GetName() < b->GetName(); });
    
    std::vector<std::string> options;
    
    for (const auto& entity : entityOptions)
    {
        options.push_back(entity->GetName());
    }
    
    int selectedEntity = pb::GuiControls::DoCombo(guiRenderMessage, PbGuiId(guiRenderMessage, item + pb::TypeHash(path.c_str())), summary, options);
    
    if (selectedEntity >= 0)
    {
        item->AcquirePointer(path)->SetPointerValue(entityOptions[selectedEntity]->GetUid());
    }
}

void PropertyUi::DoReference(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyReference* reference, ProjectStruct* item, const std::string& path)
{
    const Property* property = item->GetProperty(path);
    const PropertyReference* propertyReference = property ? property->AsReference() : 0;
    
    std::vector<ProjectRecord*> recordOptions;
    std::string summary = "<No Record>";
    
    Project* project = item->GetProject();
    const SchemaRecord* referenceType = reference->GetSchemaRecord();
    
    if (referenceType && project)
    {
        const Project::RecordMap& records = project->GetRecords();
        
        for (Project::RecordMap::const_iterator it = records.begin(); it != records.end(); ++it)
        {
            if (it->second->GetType() == referenceType)
            {
                recordOptions.push_back(it->second);

                if (propertyReference && it->second->GetUid() == propertyReference->GetReferenceValue())
                {
                    summary = it->second->GetName();
                }
            }
        }
    }
    
    std::sort(recordOptions.begin(), recordOptions.end(), [](ProjectRecord* a, ProjectRecord* b) { return a->GetName() < b->GetName(); });
    
    std::vector<std::string> options;
    
    for (const auto& record : recordOptions)
    {
        options.push_back(record->GetName());
    }
    
    int selectedRecord = pb::GuiControls::DoCombo(guiRenderMessage, PbGuiId(guiRenderMessage, item + pb::TypeHash(path.c_str())), summary, options);
    
    if (selectedRecord >= 0)
    {
        item->AcquireReference(path)->SetReferenceValue(recordOptions[selectedRecord]->GetUid());
    }
}
