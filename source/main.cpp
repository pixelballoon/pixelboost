#include <iostream>
#include <sstream>
#include <string>

#include "pugixml/pugixml.hpp"

#include "pipeline/debug/log.h"
#include "pipeline/file/fileSystem.h"

enum ShaderLanguage
{
    kShaderLanguageGLSL,
    kShaderLanguageHLSL,
};

enum ShaderType
{
    kShaderTypeVertex,
    kShaderTypeFragment,
};

bool ParseShaderGLSL(const std::string& input, pugi::xml_node& output)
{
    output.append_child(pugi::node_cdata).set_value(input.c_str());

    return true;
}

bool ParseShaderHLSL(const std::string& input, pugi::xml_node& output)
{
    output.append_child(pugi::node_cdata).set_value(input.c_str());
    
    return true;
}

bool AppendProgram(pugi::xml_node& programOutput, pugi::xml_node& programInput)
{
    std::string language = programInput.attribute("language").value();
    
    if (language == "glsl")
    {
        if (!ParseShaderGLSL(programInput.child_value(), programOutput))
            return false;
    } else if (language == "hlsl")
    {
        if (!ParseShaderHLSL(programInput.child_value(), programOutput))
            return false;
    } else {
        // Unknown language
        return false;
    }
    
    return true;
}

int main(int argc, const char * argv[])
{
    bool status = true;
    
    if (argc < 2)
        return 1;
    
    std::string vertex, fragment;
    
    const char* inputLocation = argv[1];
    const char* outputLocation = argv[2];

    pl::File* file = pl::FileSystem::Instance()->OpenFile(inputLocation, pl::kFileModeRead);
    
    PlLogInfo("shadertool", "Processing shader %s, exporting to %s", inputLocation, outputLocation);
    
    std::string input;
    
    if (!file)
        return 1;

    file->ReadAll(input);
    delete file;
    
    pugi::xml_document inputDocument;
    pugi::xml_document outputDocument;
    
    inputDocument.load(input.c_str());
    
    pugi::xml_node shaderInput = inputDocument.child("shader");
    pugi::xml_node shaderOutput = outputDocument.append_child("shader");
    shaderOutput.append_copy(shaderInput.attribute("name"));
    
    pugi::xml_node propertiesInput = shaderInput.child("properties");
    shaderOutput.append_copy(propertiesInput);
    
    pugi::xml_node techniqueInput = shaderInput.child("technique");
    
    while (!techniqueInput.empty())
    {
        pugi::xml_node techniqueOutput = shaderOutput.append_child("technique");
        
        techniqueOutput.append_copy(techniqueInput.attribute("name"));
        
        pugi::xml_node passInput = techniqueInput.child("pass");
        
        while (!passInput.empty())
        {
            pugi::xml_node passOutput = techniqueOutput.append_child("pass");
            
            pugi::xml_node programInput = passInput.child("program");
            
            while (!programInput.empty())
            {
                pugi::xml_node programOutput = passOutput.append_child("program");
                programOutput.append_copy(programInput.attribute("language"));
                status &= AppendProgram(programOutput, programInput);
                
                programInput = passInput.next_sibling("program");
            }
            
            passInput = passInput.next_sibling("pass");
        }
        
        techniqueInput = techniqueInput.next_sibling("technique");
    }
    
    std::ostringstream output;
    outputDocument.save(output);
    
    file = pl::FileSystem::Instance()->OpenFile(outputLocation, pl::kFileModeWrite);
    
    if (file)
    {
        file->Write(output.str());
        delete file;
    } else {
        status = false;
    }
    
    if (!status)
        return 1;
    
    return 0;
}
