#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Gwen/Controls/ScrollControl.h"

namespace pixeleditor
{
    
class SchemaPropertyAtom;
class Selection;
class Struct;

class PropertyPanel : public Gwen::Controls::ScrollControl
{
public:
    PropertyPanel(Gwen::Controls::Base* parent);
    ~PropertyPanel();
    
    void SetPinned(bool pinned);
    
    void SetStruct(Struct* item, const std::string& focusPath = "/");
    
private:
    glm::vec2 AddStruct(glm::vec2 offset, const SchemaStruct* schemaPropertyStruct, Struct* item, const std::string& path);
    glm::vec2 AddAtom(glm::vec2 offset, SchemaPropertyAtom* atom, Struct* item, const std::string& path);
    
    void OnSelectionChanged(const pixeleditor::Selection* selection);
    
    void OnAtomChanged(Gwen::Controls::Base* input);
    
private:
    bool _Pinned;
};

}
