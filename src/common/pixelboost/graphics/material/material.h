#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/shader/shader.h"

namespace pb
{
    
    class ShaderPass;
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
        
        Texture* GetTexture(int textureIndex);
        void SetTexture(int textureIndex, Texture* texture);
        
        void AddProperty(const std::string& name, ShaderProperty::PropertyType type);
        
        const std::map<std::string, ShaderProperty>& GetProperties();
        ShaderProperty* GetProperty(const std::string& name);
        
        ShaderPass* Bind(Uid techniqueId, int passIndex, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, float realTime, float gameTime);
        
        int GetNumPasses(Uid techniqueId);
        
    private:
        Shader* _Shader;
        Texture* _Textures[8];
        
        std::map<std::string, ShaderProperty> _Properties;
    };

};
