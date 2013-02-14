#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Gwen/Controls/ScrollControl.h"

namespace pixeleditor
{

class ProjectStruct;
class SchemaPropertyAtom;
class Selection;

class PropertyPanel : public Gwen::Controls::ScrollControl
{
public:
    PropertyPanel(Gwen::Controls::Base* parent);
    ~PropertyPanel();
    
    void SetPinned(bool pinned);
    
    void SetStruct(ProjectStruct* item, const std::string& focusPath = "/");
    
private:
    void OnStructChanged(ProjectStruct* structure, Property* property);
    
    glm::vec2 AddProperty(glm::vec2 offset, const SchemaProperty* property, ProjectStruct* item, const std::string& path);
    glm::vec2 AddStruct(glm::vec2 offset, const SchemaStruct* schemaPropertyStruct, ProjectStruct* item, const std::string& path);
    glm::vec2 AddAtom(glm::vec2 offset, const SchemaPropertyAtom* atom, ProjectStruct* item, const std::string& path);
    glm::vec2 AddPointer(glm::vec2 offset, const SchemaPropertyPointer* pointer, ProjectStruct* item, const std::string& path);
    glm::vec2 AddReference(glm::vec2 offset, const SchemaPropertyReference* reference, ProjectStruct* item, const std::string& path);
    glm::vec2 AddArray(glm::vec2 offset, const SchemaPropertyArray* array, ProjectStruct* item, const std::string& path);
    
    void OnSelectionChanged(const pixeleditor::Selection* selection);
    
    void OnAtomChanged(Gwen::Controls::Base* input);
    void OnPointerChanged(Gwen::Controls::Base* input);
    void OnReferenceChanged(Gwen::Controls::Base* input);
    
    void OnAppendArrayItem(Gwen::Controls::Base* input);
    void OnRemoveArrayItem(Gwen::Controls::Base* input);
    void OnInsertArrayItem(Gwen::Controls::Base* input);
    
private:
    bool _Pinned;
    
    ProjectStruct* _SelectedItem;
};

}
