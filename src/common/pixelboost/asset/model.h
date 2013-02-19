#pragma once

#include <vector>

#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/maths/boundingSphere.h"

namespace pb
{

    enum ModelVertexFormat
    {
        kModelVertexFormat_1P_1N_1UV,
        kModelVertexFormat_1P_1N_1UV_4BW,
    };

    class ModelVertex
    {
    public:
        ModelVertex();
        
        bool Read(ModelVertexFormat vertexFormat, File* file);
        bool Write(ModelVertexFormat vertexFormat, File* file) const;
        
    public:
        char Bone[4];
        glm::vec4 BoneWeights;
        
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Color;
        glm::vec2 UV;
    };

    class ModelMeshDefinition
    {
    public:
        ModelMeshDefinition();
        
        bool Read(File* file);
        bool Write(File* file) const;
        
        void CalculateBounds();
        
    public:
        bool Skinned;
        bool Indexed;
        pb::BoundingSphere Bounds;
        ModelVertexFormat VertexFormat;
        
        std::vector<short> Indices;
        std::vector<ModelVertex> Vertices;
    };
    
    class ModelBoneDefinition
    {
    public:
        ModelBoneDefinition();
        
        bool Read(File* file);
        bool Write(File* file) const;
    
    public:
        std::string _Name;
        
        int _Id;
        int _ParentId;
        glm::vec3 _Position;
        glm::quat _Rotation;
    
        glm::mat4x4 _BindMatrix;
    };

    class ModelAnimationDefinition
    {
    public:
        ModelAnimationDefinition();
        
        bool Read(File* file);
        bool Write(File* file) const;

    public:
        typedef std::vector<glm::mat4x4> AnimationFrame;
        
        std::string _Name;
        int _FPS;
        float _Length;
        
        std::vector<AnimationFrame> _Frames;
    };

    class ModelDefinition
    {
    public:
        ModelDefinition();
        
        bool Open(const std::string& filename);
        bool Save(const std::string& filename) const;
        
        void CalculateBounds();
        
    public:
        std::vector<ModelMeshDefinition> Meshes;
        std::vector<ModelBoneDefinition> Bones;
        std::vector<ModelAnimationDefinition> Animations;
        
    private:
        short _Version;
        
        static const short kCurrentModelVersion;
    };
    
}
