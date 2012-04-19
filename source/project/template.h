#pragma once

#include <fstream>
#include <string>

namespace pixeleditor
{

class Schema;
class SchemaEntity;
class SchemaRecord;
class SchemaStruct;
    
class TemplateGenerator
{
public:
    TemplateGenerator(Schema* schema);
    
    void GenerateTemplates(const std::string& outputDirectory);
    
private:
    void WriteTypes();
    void WriteRegister();
    
    void WriteDependencies(SchemaStruct* schemaStruct, const std::string& type, bool isHeader);
    void WriteFields(SchemaStruct* schemaStruct);
    void WriteTypeFunction(SchemaStruct* schemaStruct, bool isHeader);
    void WriteDeserialiseFunction(SchemaStruct* schemaStruct, const std::string& type, bool isHeader);
    void WriteTemplate(SchemaStruct* schemaStruct, const std::string& type);
    
    void GenerateEntity(SchemaEntity* schemaEntity);
    void GenerateRecord(SchemaRecord* schemaRecord);
    void GenerateStruct(SchemaStruct* schemaStruct);
    
private:
    std::string _OutputDirectory;
    Schema* _Schema;
    
    std::fstream _OutputHeaderFile;
    std::fstream _OutputCppFile;
};
    
}
