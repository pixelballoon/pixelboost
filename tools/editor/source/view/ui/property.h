#pragma once

#include <string>

namespace pb
{
    class GuiRenderMessage;
}

class ProjectStruct;
class SchemaPropertyArray;
class SchemaPropertyAtom;
class SchemaPropertyPointer;
class SchemaPropertyReference;
class SchemaProperty;
class SchemaStruct;

class PropertyUi
{
public:
    static void DoProperties(const pb::GuiRenderMessage& guiRenderMessage, ProjectStruct* item, const std::string& focusPath = "/");
    
private:
    static void DoStruct(const pb::GuiRenderMessage& guiRenderMessage, const SchemaStruct* schemaStruct, ProjectStruct* item, const std::string& path);
    static void DoProperty(const pb::GuiRenderMessage& guiRenderMessage, const SchemaProperty* property, ProjectStruct* item, const std::string& path);
    
    static void DoArray(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyArray* array, ProjectStruct* item, const std::string& path);
    static void DoAtom(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyAtom* atom, ProjectStruct* item, const std::string& path);
    static void DoPointer(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyPointer* pointer, ProjectStruct* item, const std::string& path);
    static void DoReference(const pb::GuiRenderMessage& guiRenderMessage, const SchemaPropertyReference* reference, ProjectStruct* item, const std::string& path);
};
