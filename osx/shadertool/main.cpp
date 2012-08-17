#include <iostream>
#include <string>

#include "pixelboost/data/xml/xml.h"

#include "hlsl2glsl.h"
#include "src/glsl/glsl_optimizer.h"

enum ShaderPlatform
{
    kShaderPlatformGLES,
    kShaderPlatformD3D9,
    kShaderPlatformOpenGL2,
};

bool CompileShader(ShaderPlatform platform, std::string input, std::string entry, std::string& output)
{
    if (platform == kShaderPlatformGLES)
    {
        // Compile Cg to GLSL and optimize
        
        ShHandle handle = Hlsl2Glsl_ConstructCompiler(EShLangVertex);
        
        Hlsl2Glsl_Parse(handle, input.c_str(), ETranslateOpUsePrecision);
        
        Hlsl2Glsl_UseUserVaryings(handle, true);
        
        EAttribSemantic semanticEnums[] = {EAttrSemPosition, EAttrSemColor0, EAttrSemColor1};
        const char* semanticNames[] = {"position", "colora", "colorb"};
        
        Hlsl2Glsl_SetUserAttributeNames(handle, semanticEnums, semanticNames, 2);
        
        Hlsl2Glsl_Translate(handle, entry.c_str(), ETranslateOpUsePrecision);
        
        const char* shader = Hlsl2Glsl_GetShader(handle);
        
        output = shader;
        
        Hlsl2Glsl_DestructCompiler(handle);
        
        glslopt_ctx* ctx = glslopt_initialize(true);
        
        glslopt_shader* optimisedShader = glslopt_optimize(ctx, kGlslOptShaderVertex, output.c_str(), 0);
        if (glslopt_get_status(optimisedShader)) {
            output = glslopt_get_output(optimisedShader);
        } else {
            printf("Error: %s\n", glslopt_get_log(optimisedShader));
        }
        glslopt_shader_delete(optimisedShader);
        
        glslopt_cleanup (ctx);

        return false;
    } else {
        // Compile using CGC
        
        fflush(0);
        
        char tempFilename[L_tmpnam];
        
        tmpnam(tempFilename);
        
        std::string command = "cgc -entry " + entry + " " + tempFilename;
        
        FILE* shaderFile = fopen(tempFilename, "w");
        
        fputs(input.c_str(), shaderFile);
        
        fclose(shaderFile);
        
        system(command.c_str());
    }
    
    return true;
}

int main(int argc, const char * argv[])
{
    std::string shader;
    
    std::string input = "";
    
    Hlsl2Glsl_Initialize();
    
    CompileShader(kShaderPlatformGLES, input, "", shader);
    
    Hlsl2Glsl_Finalize();
    
    printf("%s\n", shader.c_str());
    
    return 0;
}



