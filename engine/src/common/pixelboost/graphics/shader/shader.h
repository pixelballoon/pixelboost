#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"

namespace pugi
{
    class xml_node;
}

namespace pb
{

    class ShaderPass;
    class ShaderProgram;
    class Texture;
    
    class ShaderProperty
    {
    public:
        enum PropertyType
        {
            kPropertyUnknown,
            kPropertyTexture2D,
            kPropertyFloat,
            kPropertyVec4,
            kPropertyMat44,
        };
        
        ShaderProperty();
        ShaderProperty(PropertyType type);
        ~ShaderProperty();
        
        PropertyType GetType() const;
        
        float GetFloatValue() const;
        Texture* GetTextureValue() const;
        
        void SetValue(float value);
        void SetValue(Texture* texture);
        
    private:
        union PropertyValue
        {
            pb::Texture* Texture;
            float Float;
            float Vec4[4];
            float Mat44[4][4];
        };
        
        PropertyType _Type;
        PropertyValue _Value;
    };
    
    class ShaderTechnique
    {
    public:
        ShaderTechnique(Uid techniqueId);
        ~ShaderTechnique();
        
        Uid GetId();
        
        int GetNumPasses();
        void AddPass(ShaderPass* pass);
        ShaderPass* GetPass(int index);
        
    private:
        typedef std::vector<ShaderPass*> PassList;
        
    private:
        Uid _Uid;
        PassList _Passes;
        
        friend class Shader;
    };
        
    class ShaderPass
    {
    public:
        ShaderPass();
        ~ShaderPass();
        
        void Bind();
        void SetEngineUniforms(const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix, float realTime, float gameTime);
        
        ShaderProgram* GetShaderProgram();
        
    private:
        ShaderProgram* _Program;
    };
        
    class Shader
    {
    public:
        Shader();
        ~Shader();
        
        void AddTechnique(ShaderTechnique* technique);
        ShaderTechnique* GetTechnique(Uid techniqueId);
        
        int GetNumTechniques();
        
        const std::map<std::string, ShaderProperty::PropertyType>& GetProperties();
        
    private:
        std::map<std::string, ShaderProperty::PropertyType> _Properties;
        std::map<Uid, ShaderTechnique*> _Techniques;
    };

}
