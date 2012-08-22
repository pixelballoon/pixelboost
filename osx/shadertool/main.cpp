#include <iostream>
#include <sstream>
#include <string>

#include <Cg/cg.h>

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

bool CompileShaderCg_GLES(ShaderType type, const std::string& input, const std::string& entry, pugi::xml_node& output)
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
        "PB_Attr_Position",
        "PB_Attr_VertexPosition",
        "PB_Attr_VertexFace",
        "PB_Attr_Normal",
        "PB_Attr_Color0",
        "PB_Attr_Color1",
        "PB_Attr_Color2",
        "PB_Attr_Color3",
        "PB_Attr_Texture0",
        "PB_Attr_Texture1",
        "PB_Attr_Texture2",
        "PB_Attr_Texture3",
        "PB_Attr_Texture4",
        "PB_Attr_Texture5",
        "PB_Attr_Texture6",
        "PB_Attr_Texture7",
        "PB_Attr_Texture8",
        "PB_Attr_Texture9",
        "PB_Attr_Tangent",
        "PB_Attr_Binormal",
        "PB_Attr_BlendWeight",
        "PB_Attr_BlendIndices",
        "PB_Attr_Depth",
    };
    
    Hlsl2Glsl_SetUserAttributeNames(handle, semanticEnums, semanticNames, 23);
    
    Hlsl2Glsl_UseUserVaryings(handle, true);
    
    Hlsl2Glsl_Translate(handle, entry.c_str(), ETranslateOpUsePrecision);
    
    int count = Hlsl2Glsl_GetUniformCount(handle);
    
    const ShUniformInfo* uniforms = Hlsl2Glsl_GetUniformInfo(handle);
    for (int i = 0; i < count; i++)
    {
        //printf("UNIFORM --- %s, %s\n", uniforms[i].name, uniforms[i].semantic);
    }
    
    const char* shader = Hlsl2Glsl_GetShader(handle);
    
    glslopt_ctx* ctx = glslopt_initialize(true);
    
    glslopt_shader* optimisedShader = glslopt_optimize(ctx, type == kShaderTypeVertex ? kGlslOptShaderVertex : kGlslOptShaderFragment, shader, 0);
    if (glslopt_get_status(optimisedShader)) {
        output.append_child("source").append_child(pugi::node_cdata).set_value(glslopt_get_output(optimisedShader));
    } else {
        printf("Error: %s\n", glslopt_get_log(optimisedShader));
        return false;
    }
    glslopt_shader_delete(optimisedShader);
    
    glslopt_cleanup (ctx);
    
    Hlsl2Glsl_DestructCompiler(handle);
    
    return true;
}

bool CompileShaderCg_CGC(ShaderPlatform platform, ShaderType type, const std::string& input, const std::string& entry, pugi::xml_node& output)
{
    CGcontext context = cgCreateContext();
    
    CGprogram program;
    
    CGprofile profile;
    
    switch (platform)
    {
        case kShaderPlatformOpenGL2:
            profile = type == kShaderTypeVertex ? CG_PROFILE_GLSLV : CG_PROFILE_GLSLF;
            break;
        case kShaderPlatformD3D9_SM2:
            profile = type == kShaderTypeVertex ? CG_PROFILE_VS_2_0 : CG_PROFILE_PS_2_0;
            break;
        case kShaderPlatformD3D9_SM3:
            profile = type == kShaderTypeVertex ? CG_PROFILE_VS_3_0 : CG_PROFILE_PS_3_0;
            break;
        case kShaderPlatformGLES2:
            return false;
    }
    
    program = cgCreateProgram(context, CG_SOURCE, input.c_str(), profile, entry.c_str(), 0);
    
    cgCompileProgram(program);
    
    const char* programCompiled = cgGetProgramString(program, CG_COMPILED_PROGRAM);
    
    CGparameter param = cgGetFirstParameter(program, CG_PROGRAM);
    while (param)
    {
        if (cgGetParameterDirection(param) == CG_IN)
        {
            //printf("PARAM --- %s [%s] (%lu)\n", cgGetParameterName(param), cgGetParameterSemantic(param), cgGetParameterResourceIndex(param));
        }
        
        param = cgGetNextParameter(param);
    }
    
    cgDestroyProgram(program);
    cgDestroyContext(context);
    
    output.append_child("source").append_child(pugi::node_cdata).set_value(programCompiled);
    
    return true;
}

bool CompileShaderCg(ShaderPlatform platform, const std::string& input, const std::string& vertexEntry, const std::string& fragmentEntry, pugi::xml_node& vertex, pugi::xml_node& fragment)
{
    bool status = true;
    
    if (platform == kShaderPlatformGLES2)
    {
        status &= CompileShaderCg_GLES(kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderCg_GLES(kShaderTypeFragment, input, fragmentEntry, fragment);
    } else {
        status &= CompileShaderCg_CGC(platform, kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderCg_CGC(platform, kShaderTypeFragment, input, fragmentEntry, fragment);
    }
    
    return status;
}

//bool CompileShaderGLSL(ShaderType type, const std::string& input, pugi::xml_node& output)
bool CompileShaderGLSL(ShaderPlatform platform, const std::string& input, pugi::xml_node& vertex, pugi::xml_node& fragment)
{
    if (platform == kShaderPlatformGLES2 || platform == kShaderPlatformOpenGL2)
    {
        vertex.append_child("source").append_child(pugi::node_cdata).set_value(input.c_str());
        fragment.append_child("source").append_child(pugi::node_cdata).set_value(input.c_str());
        return true;
    }
    
    return false;
}

bool AppendProgram(ShaderPlatform platform, pugi::xml_node& programOutput, pugi::xml_node& programInput)
{
    pugi::xml_node fragmentProgramOutput = programOutput.append_child("fragmentProgram");
    pugi::xml_node vertexProgramOutput = programOutput.append_child("vertexProgram");
    
    std::string language = programInput.attribute("language").value();
    
    if (language == "cg")
    {
        std::string vertexEntry = "vert";
        std::string fragmentEntry = "frag";
        
        if (!CompileShaderCg(platform, programInput.child_value(), vertexEntry, fragmentEntry, vertexProgramOutput, fragmentProgramOutput))
            return false;
    } else if (language == "glsl")
    {
        if (!CompileShaderGLSL(platform, programInput.child_value(), vertexProgramOutput, fragmentProgramOutput))
            return false;
    } else {
        // Unknown language
        return false;
    }
    
    return true;
}

int main(int argc, const char * argv[])
{
    Hlsl2Glsl_Initialize();
    
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
        
            pugi::xml_node programInput = passInput.child("program");
            
            while (!programInput.empty())
            {
                pugi::xml_node passOutput = subshaderOutput.append_child("pass");
                
                {
                    pugi::xml_node programOutput = passOutput.append_child("program");
                    programOutput.append_attribute("type").set_value("d3d9_sm3");
                    status &= AppendProgram(kShaderPlatformD3D9_SM3, programOutput, programInput);
                }
                
                {
                    pugi::xml_node programOutput = passOutput.append_child("program");
                    programOutput.append_attribute("type").set_value("d3d9_sm2");
                    status &= AppendProgram(kShaderPlatformD3D9_SM2, programOutput, programInput);
                }
                
                {
                    pugi::xml_node programOutput = passOutput.append_child("program");
                    programOutput.append_attribute("type").set_value("gl2");
                    status &= AppendProgram(kShaderPlatformOpenGL2, programOutput, programInput);
                }
                
                {
                    pugi::xml_node programOutput = passOutput.append_child("program");
                    programOutput.append_attribute("type").set_value("gles2");
                    status &= AppendProgram(kShaderPlatformGLES2, programOutput, programInput);
                }
                
                programInput = passInput.next_sibling("program");
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
