#include <iostream>
#include <sstream>
#include <string>

#include "pixelboost/data/xml/xml.h"
#include "pixelboost/file/fileHelpers.h"

#include "hlsl2glsl.h"
#include "src/glsl/glsl_optimizer.h"

enum ShaderPlatform
{
    kShaderPlatformGLES2,
    kShaderPlatformD3D9_SM2,
    kShaderPlatformD3D9_SM3,
    kShaderPlatformOpenGL2,
};

enum ShaderType
{
    kShaderTypeVertex,
    kShaderTypeFragment,
};

bool CompileShaderGLES(ShaderType type, const std::string& input, const std::string& entry, std::string& output)
{
    // Compile Cg to GLSL and optimize
    
    ShHandle handle = Hlsl2Glsl_ConstructCompiler(type == kShaderTypeVertex ? EShLangVertex : EShLangFragment);
    
    Hlsl2Glsl_Parse(handle, input.c_str(), ETranslateOpUsePrecision);
    
    EAttribSemantic semanticEnums[] = {
        EAttrSemPosition,
        EAttrSemVPos,
        EAttrSemVFace,
        EAttrSemNormal,
        EAttrSemColor0,
        EAttrSemColor1,
        EAttrSemColor2,
        EAttrSemColor3,
        EAttrSemTex0,
        EAttrSemTex1,
        EAttrSemTex2,
        EAttrSemTex3,
        EAttrSemTex4,
        EAttrSemTex5,
        EAttrSemTex6,
        EAttrSemTex7,
        EAttrSemTex8,
        EAttrSemTex9,
        EAttrSemTangent,
        EAttrSemBinormal,
        EAttrSemBlendWeight,
        EAttrSemBlendIndices,
        EAttrSemDepth,
    };
    
    const char* semanticNames[] = {
        "__Position",
        "__VertexPosition",
        "__VertexFace",
        "__Normal",
        "__Color0",
        "__Color1",
        "__Color2",
        "__Color3",
        "__Tex0",
        "__Tex1",
        "__Tex2",
        "__Tex3",
        "__Tex4",
        "__Tex5",
        "__Tex6",
        "__Tex7",
        "__Tex8",
        "__Tex9",
        "__Tangent",
        "__Binormal",
        "__BlendWeight",
        "__BlendIndices",
        "__Depth",
    };
    
    Hlsl2Glsl_SetUserAttributeNames(handle, semanticEnums, semanticNames, 23);
    
    Hlsl2Glsl_UseUserVaryings(handle, true);
    
    Hlsl2Glsl_Translate(handle, entry.c_str(), ETranslateOpUsePrecision);
    
    const char* shader = Hlsl2Glsl_GetShader(handle);
    
    output = shader;
    
    Hlsl2Glsl_DestructCompiler(handle);
    
    glslopt_ctx* ctx = glslopt_initialize(true);
    
    glslopt_shader* optimisedShader = glslopt_optimize(ctx, type == kShaderTypeVertex ? kGlslOptShaderVertex : kGlslOptShaderFragment, output.c_str(), 0);
    if (glslopt_get_status(optimisedShader)) {
        output = glslopt_get_output(optimisedShader);
    } else {
        printf("Error: %s\n", glslopt_get_log(optimisedShader));
        return false;
    }
    glslopt_shader_delete(optimisedShader);
    
    glslopt_cleanup (ctx);
    
    return true;
}

bool CompileShaderCGC(ShaderPlatform platform, ShaderType type, const std::string& input, const std::string& entry, std::string& output)
{
    fflush(0);
    
    char tempInputFilename[L_tmpnam];
    char tempOutputFilename[L_tmpnam];
    
    tmpnam(tempInputFilename);
    tmpnam(tempOutputFilename);
    
    std::string profile;
    
    switch (platform)
    {
        case kShaderPlatformOpenGL2:
            profile = type == kShaderTypeVertex ? "arbvp1" : "arbfp1";
            break;
        case kShaderPlatformD3D9_SM2:
            profile = type == kShaderTypeVertex ? "vs_2_0" : "ps_2_0";
            break;
        case kShaderPlatformD3D9_SM3:
            profile = type == kShaderTypeVertex ? "vs_3_0" : "ps_3_0";
            break;
        case kShaderPlatformGLES2:
            break;
    }
    
    FILE* shaderFile = fopen(tempInputFilename, "w");
    
    fputs(input.c_str(), shaderFile);
    
    fclose(shaderFile);
    
    std::string command = "cgc -entry " + entry + " -profile " + profile + " " + tempInputFilename + " -o " + tempOutputFilename;
    
    system(command.c_str());
    
    FILE* cgcOutput = fopen(tempOutputFilename, "r");
    
    if (cgcOutput)
    {
        char buffer[1024];
        while (char* line = fgets(buffer, sizeof(buffer), cgcOutput))
        {
            output += line;
        }
        
        fclose(cgcOutput);
    } else {
        return false;
    }
    
    remove(tempInputFilename);
    remove(tempOutputFilename);
    
    return true;
}

bool CompileShader(ShaderPlatform platform, const std::string& input, const std::string& vertexEntry, const std::string& fragmentEntry, std::string& vertex, std::string& fragment)
{
    bool status = true;
    
    if (platform == kShaderPlatformGLES2)
    {
        status &= CompileShaderGLES(kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderGLES(kShaderTypeFragment, input, fragmentEntry, fragment);
    } else {
        status &= CompileShaderCGC(platform, kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderCGC(platform, kShaderTypeFragment, input, fragmentEntry, fragment);
    }
    
    return status;
}

bool AppendProgram(ShaderPlatform platform, pugi::xml_node& programOutput, pugi::xml_node& passInput)
{
    pugi::xml_node fragmentProgramOutput = programOutput.append_child("fragmentProgram");
    pugi::xml_node vertexProgramOutput = programOutput.append_child("vertexProgram");
    
    std::string vertexEntry = "vert";
    std::string fragmentEntry = "frag";
    
    std::string vertex;
    std::string fragment;
    
    if (CompileShader(platform, passInput.child_value(), vertexEntry, fragmentEntry, vertex, fragment))
    {
        vertexProgramOutput.append_child(pugi::node_cdata).set_value(vertex.c_str());
        fragmentProgramOutput.append_child(pugi::node_cdata).set_value(fragment.c_str());

        return true;
    }
    
    return false;
}

int main(int argc, const char * argv[])
{
    Hlsl2Glsl_Initialize();
    
    bool status;
    
    std::string vertex, fragment;
    
    const char* inputLocation = argc > 1 ? argv[1] : "";
    
    const char* outputLocation = argc > 2 ? argv[2] : "";

    std::string input = pb::FileHelpers::FileToString(pb::kFileLocationUser, inputLocation);
    
    pugi::xml_document inputDocument;
    pugi::xml_document outputDocument;
    
    inputDocument.load(input.c_str());
    
    pugi::xml_node shaderInput = inputDocument.child("shader");
    pugi::xml_node shaderOutput = outputDocument.append_child("shader");
    
    pugi::xml_node attributesInput = shaderInput.child("attributes");
    shaderOutput.append_copy(attributesInput);
    
    pugi::xml_node subshaderInput = shaderInput.child("subshader");
    
    while (!subshaderInput.empty())
    {
        pugi::xml_node subshaderOutput = shaderOutput.append_child("subshader");
        
        pugi::xml_node passInput = subshaderInput.child("pass");
        
        while (!passInput.empty())
        {
            pugi::xml_node passOutput = subshaderOutput.append_child("pass");
            
            {
                pugi::xml_node programOutput = passOutput.append_child("program");
                programOutput.append_attribute("type").set_value("d3d9_sm3");
                AppendProgram(kShaderPlatformD3D9_SM3, programOutput, passInput);
            }
            
            {
                pugi::xml_node programOutput = passOutput.append_child("program");
                programOutput.append_attribute("type").set_value("d3d9_sm2");
                AppendProgram(kShaderPlatformD3D9_SM2, programOutput, passInput);
            }
            
            {
                pugi::xml_node programOutput = passOutput.append_child("program");
                programOutput.append_attribute("type").set_value("gl2");
                AppendProgram(kShaderPlatformOpenGL2, programOutput, passInput);
            }
            
            {
                pugi::xml_node programOutput = passOutput.append_child("program");
                programOutput.append_attribute("type").set_value("gles2");
                AppendProgram(kShaderPlatformGLES2, programOutput, passInput);
            }
            
            passInput = passInput.next_sibling("pass");
        }
        
        subshaderInput = subshaderInput.next_sibling("subshader");
    }
    
    std::ostringstream output;
    outputDocument.save(output);
    
    pb::FileHelpers::StringToFile(pb::kFileLocationUser, outputLocation, output.str());
    
    Hlsl2Glsl_Finalize();
    
    if (!status)
        return 1;
    
    return 0;
}
