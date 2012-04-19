#include "project/record.h"
#include "project/schema.h"
#include "project/template.h"

using namespace pixeleditor;
    
TemplateGenerator::TemplateGenerator(Schema* schema)
    : _Schema(schema)
{
    
}
    
void TemplateGenerator::GenerateTemplates(const std::string& outputDirectory)
{
    _OutputDirectory = outputDirectory;
    
    WriteTypes();
    WriteRegister();
    
    for (Schema::StructMap::const_iterator it = _Schema->GetStructs().begin(); it != _Schema->GetStructs().end(); ++it)
    {
        std::string camelCaseName = it->second->GetName();
        camelCaseName[0] = tolower(camelCaseName[0]);
        
        std::string cpp = outputDirectory + camelCaseName + ".cpp";
        std::string header = outputDirectory + camelCaseName + ".h";
        
        _OutputCppFile.open(cpp.c_str(), std::ios_base::out);
        
        if (!_OutputCppFile.is_open())
            continue;
        
        _OutputHeaderFile.open(header.c_str(), std::ios_base::out);
        
        if (!_OutputHeaderFile.is_open())
        {
            _OutputCppFile.close();
            continue;
        }
        
        switch (it->second->GetSchemaType())
        {
            case SchemaItem::kSchemaRecord:
                GenerateRecord(static_cast<SchemaRecord*>(it->second));
                break;
                
            case SchemaItem::kSchemaStruct:
                GenerateStruct(static_cast<SchemaStruct*>(it->second));
                break;
                
            case SchemaItem::kSchemaEntity:
                GenerateEntity(static_cast<SchemaEntity*>(it->second));
                break;
                
            default:
                break;
        }
        
        _OutputCppFile.close();
        _OutputHeaderFile.close();
    }
}
    
void TemplateGenerator::WriteTypes()
{
    std::fstream header;
    
    header.open((_OutputDirectory + "types.h").c_str(), std::ios_base::out);
    
    if (!header.is_open())
        return;
    
    header << "#pragma once\n\n";
    header << "enum Types\n{\n";
    
    header << "    kDbStruct = 0,\n";
    header << "    kDbEntity = 1,\n";
    header << "    kDbRecord = 2,\n";    
    
    for (Schema::StructMap::const_iterator it = _Schema->GetStructs().begin(); it != _Schema->GetStructs().end(); ++it)
    {
        header << "    kDb" << it->second->GetName() << ",\n";
    }
    
    header << "    kDbCount,\n";
    
    header << "};\n";
    
    header.close();
}
    
void TemplateGenerator::WriteRegister()
{
    std::fstream header, cpp;
    
    header.open((_OutputDirectory + "register.h").c_str(), std::ios_base::out);
    cpp.open((_OutputDirectory + "register.cpp").c_str(), std::ios_base::out);
    
    if (!header.is_open())
        return;
    
    if (!cpp.is_open())
    {
        header.close();
        return;
    }
    
    header << "#pragma once\n\n";
    header << "namespace libpixeldb\n{\n\n";
    header << "void RegisterTypes();\n\n";
    header << "}\n";
    
    cpp << "#include \"register.h\"\n";
    cpp << "#include \"db/manager.h\"\n\n";
    
    for (Schema::StructMap::const_iterator it = _Schema->GetStructs().begin(); it != _Schema->GetStructs().end(); ++it)
    {
        std::string camelCaseName = it->second->GetName();
        camelCaseName[0] = tolower(camelCaseName[0]);
        cpp << "#include \"" << camelCaseName << ".h\"\n";
    }
    
    cpp << "\nnamespace libpixeldb\n{\n\n";
    
    for (Schema::StructMap::const_iterator it = _Schema->GetStructs().begin(); it != _Schema->GetStructs().end(); ++it)
    {
        cpp << "Struct* Create" << it->second->GetName() << "()\n{\n";
        cpp << "    return new " << it->second->GetName() << "();\n}\n\n";
    }
    
    cpp << "void RegisterTypes()\n{\n";
    for (Schema::StructMap::const_iterator it = _Schema->GetStructs().begin(); it != _Schema->GetStructs().end(); ++it)
    {
        cpp << "    DatabaseManager::Instance()->RegisterStruct(\"" << it->second->GetName() << "\", &Create" << it->second->GetName() << ");\n";
    }
    
    cpp << "}\n\n";
    cpp << "}\n";
    
    cpp.close();
    header.close();

}
    
void TemplateGenerator::WriteDependencies(SchemaStruct* schemaStruct, const std::string& type, bool isHeader)
{
    std::fstream& output = isHeader ? _OutputHeaderFile : _OutputCppFile;
    
    bool hasString = false;
    bool hasArray = false;
    bool hasPointer = false;
    
    output << "#include \"types.h\"\n";
    
    const SchemaStruct* baseType = schemaStruct->GetBaseType();
    
    std::string camelType = type;
    camelType[0] = tolower(camelType[0]);
    std::string baseInclude = (baseType ? baseType->GetName() : (std::string("db/") + camelType)) + ".h";
    baseInclude[0] = tolower(baseInclude[0]);
    
    output << "#include \"" << baseInclude << "\"\n";
    
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {   
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                SchemaPropertyAtom* schemaAtom = static_cast<SchemaPropertyAtom*>(it->second);
                
                if (schemaAtom->GetAtomType() == SchemaPropertyAtom::kSchemaAtomString)
                    hasString = true;
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyArray:
            {
                hasArray = true;
                
                SchemaPropertyArray* schemaArray = static_cast<SchemaPropertyArray*>(it->second);
                const SchemaPropertyStruct* schemaStruct = schemaArray->GetSchemaStruct();                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (!s)
                    continue;
                
                std::string camelCaseName = s->GetName();
                camelCaseName[0] = tolower(camelCaseName[0]);
                
                output << "#include \"" << camelCaseName << ".h\"\n";
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* schemaStruct = static_cast<SchemaPropertyStruct*>(it->second);
                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (!s)
                    continue;
                
                std::string camelCaseName = s->GetName();
                camelCaseName[0] = tolower(camelCaseName[0]);
                
                output << "#include \"" << camelCaseName << ".h\"\n";
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyPointer:
            {
                hasPointer = true;
                
                SchemaPropertyPointer* schemaPointer = static_cast<SchemaPropertyPointer*>(it->second);
                
                const SchemaEntity* entity = schemaPointer->GetSchemaEntity();
                
                if (!entity)
                    continue;
                
                std::string camelCaseName = entity->GetName();
                camelCaseName[0] = tolower(camelCaseName[0]);
                
                output << "#include \"" << camelCaseName << ".h\"\n";
                
                break;
            }
        }
    }
    
    output << "#include \"json/reader.h\"\n";
    
    if (hasString)
        output << "#include <string>\n";
        
    if (hasArray)
        output << "#include <vector>\n";
    
    if (hasPointer)
        output << "#include \"db/record.h\"\n";
}
    
void TemplateGenerator::WriteFields(SchemaStruct* schemaStruct)
{
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        std::string prefix = "";
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                SchemaPropertyAtom* schemaAtom = static_cast<SchemaPropertyAtom*>(it->second);
                
                switch (schemaAtom->GetAtomType())
                {
                    case SchemaPropertyAtom::kSchemaAtomFloat:
                        prefix = "float";
                        break;
                    case SchemaPropertyAtom::kSchemaAtomInt:
                        prefix = "int";
                        break;
                    case SchemaPropertyAtom::kSchemaAtomString:
                        prefix = "std::string";
                        break;
                }
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyArray:
            {
                SchemaPropertyArray* schemaArray = static_cast<SchemaPropertyArray*>(it->second);
                const SchemaPropertyStruct* schemaStruct = schemaArray->GetSchemaStruct();                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (!s)
                    continue;
                
                prefix = std::string("std::vector<") + s->GetName() + ">";
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* schemaStruct = static_cast<SchemaPropertyStruct*>(it->second);
                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (!s)
                    continue;
                
                prefix = s->GetName();
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyPointer:
            {
                SchemaPropertyPointer* schemaPointer = static_cast<SchemaPropertyPointer*>(it->second);
                
                const SchemaEntity* entity = schemaPointer->GetSchemaEntity();
                
                if (!entity)
                    continue;
                
                prefix = entity->GetName() + "*";
                
                break;
            }
        }
        
        _OutputHeaderFile << "    " << prefix << " _" << it->second->GetName() << ";\n";
    }
}
    
void TemplateGenerator::WriteTypeFunction(SchemaStruct* schemaStruct, bool isHeader)
{
    if (isHeader)
    {
        _OutputHeaderFile << "    virtual int GetType();\n";
        return;
    }
    
    _OutputCppFile << "int " << schemaStruct->GetName() << "::GetType()\n{\n";
    _OutputCppFile << "    return kDb" << schemaStruct->GetName() << ";\n";
    _OutputCppFile << "}\n\n";
}
    
void TemplateGenerator::WriteDeserialiseFunction(SchemaStruct* schemaStruct, const std::string& type, bool isHeader)
{
    if (isHeader)
    {
        _OutputHeaderFile << "    virtual void Deserialise(json::Object& container, Record* context);\n";
        return;
    }
    
    _OutputCppFile << "void " << schemaStruct->GetName() << "::Deserialise(json::Object& container, Record* context)\n{\n";
    
    const SchemaStruct* baseType = schemaStruct->GetBaseType();
    
    _OutputCppFile << "    " << (baseType ? baseType->GetName() : type) << "::Deserialise(container, context);\n\n";
    
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        // Initialise pointers to 0 to prevent null reference errors
        if (it->second->GetPropertyType() == SchemaProperty::kSchemaPropertyPointer)
        {
            _OutputCppFile << "    _" << it->second->GetName() << " = 0; \n";
        }
        
        _OutputCppFile << "    if (container.Find(\"" << it->second->GetName() << "\") != container.End())\n    {\n";
        
        switch (it->second->GetPropertyType())
        {
            case SchemaProperty::kSchemaPropertyAtom:
            {
                SchemaPropertyAtom* schemaAtom = static_cast<SchemaPropertyAtom*>(it->second);
                
                switch (schemaAtom->GetAtomType())
                {
                    case SchemaPropertyAtom::kSchemaAtomFloat:
                    case SchemaPropertyAtom::kSchemaAtomInt:
                        _OutputCppFile << "        json::Number& number = container[\"" << schemaAtom->GetName() << "\"];\n";
                        _OutputCppFile << "        _" << schemaAtom->GetName() << " = number.Value();\n";
                        break;
                    
                    case SchemaPropertyAtom::kSchemaAtomString:
                        _OutputCppFile << "        json::String& string = container[\"" << schemaAtom->GetName() << "\"];\n";
                        _OutputCppFile << "        _" << schemaAtom->GetName() << " = string.Value();\n";
                        break;
                }
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyArray:
            {
                SchemaPropertyArray* schemaArray = static_cast<SchemaPropertyArray*>(it->second);
                const SchemaPropertyStruct* schemaStruct = schemaArray->GetSchemaStruct();                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (s)
                {
                    _OutputCppFile << "        json::Array& array = container[\"" << schemaArray->GetName() << "\"];\n";
                    _OutputCppFile << "        for (json::Array::iterator it = array.Begin(); it!=array.End(); ++it)\n        {\n";
                    _OutputCppFile << "            _" << schemaArray->GetName() << ".push_back(" << s->GetName() << "());\n";
                    _OutputCppFile << "            json::Object& item = *it;\n";
                    _OutputCppFile << "            _" << schemaArray->GetName() << ".back().Deserialise(item, context);\n";
                    _OutputCppFile << "        }\n";
                }
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyStruct:
            {
                SchemaPropertyStruct* schemaStruct = static_cast<SchemaPropertyStruct*>(it->second);
                
                const SchemaStruct* s = schemaStruct->GetSchemaStruct();
                
                if (s)
                {
                    _OutputCppFile << "        json::Object& item = container[\"" << schemaStruct->GetName() << "\"];\n";
                    _OutputCppFile << "        _" << schemaStruct->GetName() << ".Deserialise(item, context);\n";
                }
                
                break;
            }
                
            case SchemaProperty::kSchemaPropertyPointer:
            {
                SchemaPropertyPointer* schemaPointer = static_cast<SchemaPropertyPointer*>(it->second);
                
                const SchemaEntity* entity = schemaPointer->GetSchemaEntity();
                
                if (entity)
                {
                    _OutputCppFile << "        json::Number& pointer = container[\"" << schemaPointer->GetName() << "\"];\n";
                    _OutputCppFile << "        context->AddPointer(pointer.Value(), (void**)&_" << schemaPointer->GetName() << ");\n";
                }
                
                break;
            }
        }
        
        _OutputCppFile << "    }\n\n";
    }
    
    _OutputCppFile.seekp(-1, std::ios_base::cur);
    
    _OutputCppFile << "}\n";
}
    
void TemplateGenerator::WriteTemplate(SchemaStruct* schemaStruct, const std::string& type)
{
    // .h
    
    std::string upperName = schemaStruct->GetName();
    std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
    
    _OutputHeaderFile << "#pragma once\n\n";
    
    WriteDependencies(schemaStruct, type, true);
    
    _OutputHeaderFile << "\nnamespace libpixeldb\n{\n\n";
    
    const SchemaStruct* baseType = schemaStruct->GetBaseType();
    
    _OutputHeaderFile << "class " << schemaStruct->GetName() << " : public " << (baseType ? baseType->GetName() : type) << std::endl;
    
    _OutputHeaderFile << "{" << std::endl << "public:\n";
    
    WriteFields(schemaStruct);
    
    _OutputHeaderFile << std::endl << "public:\n";
    
    WriteTypeFunction(schemaStruct, true);
    WriteDeserialiseFunction(schemaStruct, type, true);
    
    _OutputHeaderFile << "};\n\n";
    
    _OutputHeaderFile << "}\n";
    
    // .cpp
    
    std::string camelType = type;
    camelType[0] = tolower(camelType[0]);
    std::string headerInclude = (schemaStruct ? schemaStruct->GetName() : (std::string("db/") + camelType)) + ".h";
    headerInclude[0] = tolower(headerInclude[0]);
    
    _OutputCppFile << "#include \"" << headerInclude << "\"\n\n";
    
    // This will be needed again once forward declaration is properly supported
//  WriteDependencies(schemaStruct, type, false);

    _OutputCppFile << "namespace libpixeldb\n{\n\n";
    
    WriteTypeFunction(schemaStruct, false);
    WriteDeserialiseFunction(schemaStruct, type, false);
    
    _OutputCppFile << "\n}\n";
}
    
void TemplateGenerator::GenerateRecord(SchemaRecord* schemaRecord)
{
    WriteTemplate(schemaRecord, "Record");
}
    
void TemplateGenerator::GenerateStruct(SchemaStruct* schemaStruct)
{
    WriteTemplate(schemaStruct, "Struct");
}

void TemplateGenerator::GenerateEntity(SchemaEntity* schemaEntity)
{
    WriteTemplate(schemaEntity, "Entity");
}
