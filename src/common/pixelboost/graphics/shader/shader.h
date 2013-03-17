#pragma once

#include <map>
#include <string>
#include <vector>

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
            kPropertyTexture2D,
            kPropertyFloat,
            kPropertyVec4,
            kPropertyMat44,
        };
        
        ShaderProperty(const std::string& name);
        ~ShaderProperty();
        
        const std::string& GetName();
        
        void SetValue(float value);
        
        float GetFloatValue();
        
    private:
        union PropertyValue
        {
            Texture* Texture;
            float Float;
            float Vec4[4];
            float Mat44[4][4];
        };
        
        std::string _Name;
        PropertyValue _Value;
    };
    
    class ShaderTechnique
    {
    public:
        ShaderTechnique();
        ~ShaderTechnique();
        
        Uid GetId();
        
        bool Load(const pugi::xml_node& attributes, const pugi::xml_node& technique);
        
        int GetNumPasses();
        void AddPass(ShaderPass* pass);
        ShaderPass* GetPass(int index);
        
    private:
        typedef std::vector<ShaderPass*> PassList;
        
    private:
        Uid _Uid;
        PassList _Passes;
    };
        
    class ShaderPass
    {
    public:
        ShaderPass();
        ~ShaderPass();
        
        bool Load(const pugi::xml_node& attributes, const pugi::xml_node& pass);
        
        void Bind();
        
        ShaderProgram* GetShaderProgram();
        
    private:
        ShaderProgram* _Program;
    };
        
    class Shader
    {
    public:
        Shader();
        ~Shader();
        
        bool Load(const std::string& filename);
        
        void AddTechnique(ShaderTechnique* technique);
        ShaderTechnique* GetTechnique(Uid techniqueId);
        
    private:
        typedef std::map<Uid, ShaderTechnique*> TechniqueMap;
        
        TechniqueMap _Techniques;
    };

}
