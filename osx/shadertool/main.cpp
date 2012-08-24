#include <iostream>
#include <sstream>
#include <string>

#include <Cg/cg.h>

#include "pixelboost/data/xml/xml.h"
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
    
    std::string vertex, fragment;
    
    const char* inputLocation = argc > 1 ? argv[1] : "";
    
    const char* outputLocation = argc > 2 ? argv[2] : "";

    std::string input = pb::FileHelpers::FileToString(pb::kFileLocationUser, inputLocation);
    
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
    
    pb::FileHelpers::StringToFile(pb::kFileLocationUser, outputLocation, output.str());
    
    if (!status)
        return 1;
    
    return 0;
}
