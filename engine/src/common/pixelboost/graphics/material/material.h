#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/device/device.h"
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
        ShaderPass* Bind(Uid techniqueId, int passIndex, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
        int GetNumPasses(Uid techniqueId);
        
        Shader* GetShader();
        void SetShader(Shader* shader);
        
        bool GetBlendEnabled();
        void SetBlendEnabled(bool enabled);
        
        std::pair<GraphicsDevice::Blend, GraphicsDevice::Blend> GetBlendMode();
        void SetBlendMode(GraphicsDevice::Blend source, GraphicsDevice::Blend destination);
        
        bool GetDepthTestEnabled();
        void SetDepthTestEnabled(bool enabled);

        ShaderProperty& AddProperty(const std::string& name, ShaderProperty::PropertyType type);
        const std::map<std::string, ShaderProperty>& GetProperties();
        ShaderProperty* GetProperty(const std::string& name);
        
    private:
        bool _Blend;
        std::pair<GraphicsDevice::Blend, GraphicsDevice::Blend> _BlendMode;
        bool _DepthTest;
        
        Shader* _Shader;
        
        std::map<std::string, ShaderProperty> _Properties;
    };

};
