#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"

namespace pb
{
    
    class ShaderProperty;
    class Shader;
    class Texture;

    class Material
    {
    public:
        Material();
        ~Material();

    public:
        Shader* GetShader();
        void SetShader(Shader* shader);
        
        const std::map<std::string, ShaderProperty>& GetProperties();
        ShaderProperty* GetProperty(const std::string& name);
        
        void Bind(Uid techniqueId, int pass, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, float gameTime, float realTime);
        
        int GetNumPasses(Uid techniqueId);
        
    private:
        Shader* _Shader;
        
        std::map<std::string, ShaderProperty> _Properties;
    };

};
