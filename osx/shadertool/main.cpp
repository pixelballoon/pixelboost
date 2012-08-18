#include <iostream>
#include <string>

#include "pixelboost/data/xml/xml.h"

#include "hlsl2glsl.h"
#include "src/glsl/glsl_optimizer.h"

enum ShaderPlatform
{
    kShaderPlatformGLES,
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
    
    Hlsl2Glsl_UseUserVaryings(handle, true);
    
    EAttribSemantic semanticEnums[] = {EAttrSemPosition, EAttrSemBinormal, EAttrSemNormal, EAttrSemColor0, EAttrSemColor1};
    const char* semanticNames[] = {"position", "binormal", "normal", "colora", "colorb"};
    
    Hlsl2Glsl_SetUserAttributeNames(handle, semanticEnums, semanticNames, 5);
    
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
    
    char tempFilename[L_tmpnam];
    
    tmpnam(tempFilename);
    
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
        case kShaderPlatformGLES:
            break;
    }
    
    FILE* shaderFile = fopen(tempFilename, "w");
    
    fputs(input.c_str(), shaderFile);
    
    fclose(shaderFile);
    
    std::string command = "cgc -entry " + entry + " -profile " + profile + " " + tempFilename + " 2>0";
    
    FILE* cgcOutput = popen(command.c_str(), "r");
    
    char buffer[1024];
    while (char* line = fgets(buffer, sizeof(buffer), cgcOutput))
    {
        output += line;
    }
    
    pclose(cgcOutput);
    
    remove(tempFilename);
    
    return true;
}

bool CompileShader(ShaderPlatform platform, const std::string& input, const std::string& vertexEntry, const std::string& fragmentEntry, std::string& vertex, std::string& fragment)
{
    bool status = true;
    
    if (platform == kShaderPlatformGLES)
    {
        status &= CompileShaderGLES(kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderGLES(kShaderTypeFragment, input, fragmentEntry, fragment);
    } else {
        status &= CompileShaderCGC(platform, kShaderTypeVertex, input, vertexEntry, vertex);
        status &= CompileShaderCGC(platform, kShaderTypeFragment, input, fragmentEntry, fragment);
    }
    
    return status;
}

int main(int argc, const char * argv[])
{
    std::string vertex, fragment;
    
    std::string input = "";
    
    std::string vertexEntry = "vert";
    std::string fragmentEntry = "frag";
    
    Hlsl2Glsl_Initialize();
    
    bool status = true;
    
    status &= CompileShader(kShaderPlatformOpenGL2, input, vertexEntry, fragmentEntry, vertex, fragment);
    
    Hlsl2Glsl_Finalize();
    
    if (!status)
        return 1;
    
    return 0;
}



