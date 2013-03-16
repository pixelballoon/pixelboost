#pragma once

#include <map>
#include <string>

namespace pb
{
    
    class Shader;
    
    class ShaderManager
    {
    public:
        ShaderManager();
        ~ShaderManager();
        
        Shader* LoadShader(const std::string& file, bool reload = false);
        void UnloadShader(const std::string& file);
        
        Shader* GetShader(const std::string& file);
        
    private:
        typedef std::map<std::string, Shader*> ShaderMap;
        ShaderMap _Shaders;
    };

}
