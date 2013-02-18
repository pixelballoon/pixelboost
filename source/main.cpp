#include <iostream>
#include <sstream>
#include <string>

#include "pixelboost/data/xml/pugixml.hpp"
#include "pixelboost/file/fileHelpers.h"

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
    
    if (argc < 3)
        return 1;
    
    new pb::FileSystem(argv[0]);
    
    pb::FileSystem::Instance()->MountReadLocation(argv[1], "/", true);
    pb::FileSystem::Instance()->OverrideWriteDirectory(argv[1]);
    
    std::string vertex, fragment;
    
    const char* inputLocation = argv[2];
    const char* outputLocation = argv[3];

    pb::File* file = pb::FileSystem::Instance()->OpenFile(inputLocation);
    
    std::string input;
    
    if (file)
    {
        file->ReadAll(input);
        delete file;
    }
    
    pugi::xml_document inputDocument;
    pugi::xml_document outputDocument;
    
    inputDocument.load(input.c_str());
    
    pugi::xml_node shaderInput = inputDocument.child("shader");
    pugi::xml_node shaderOutput = outputDocument.append_child("shader");
    shaderOutput.append_copy(shaderInput.attribute("name"));
    
    pugi::xml_node attributesInput = shaderInput.child("attributes");
    shaderOutput.append_copy(attributesInput);
    
    pugi::xml_node subshaderInput = shaderInput.child("subshader");
    
    while (!subshaderInput.empty())
    {
        pugi::xml_node subshaderOutput = shaderOutput.append_child("subshader");
        
        subshaderOutput.append_copy(subshaderInput.attribute("class"));
        
        pugi::xml_node passInput = subshaderInput.child("pass");
        
        while (!passInput.empty())
        {
            pugi::xml_node passOutput = subshaderOutput.append_child("pass");
            
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
        
        subshaderInput = subshaderInput.next_sibling("subshader");
    }
    
    std::ostringstream output;
    outputDocument.save(output);
    
    file = pb::FileSystem::Instance()->OpenFile(outputLocation, pb::kFileModeWrite);
    
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
