#pragma once

#include <string>
#include <vector>

#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

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
        
        void CalculateBounds();
        
    public:
        std::vector<ModelMeshDefinition> Meshes;
        std::vector<ModelBoneDefinition> Bones;
        std::vector<ModelAnimationDefinition> Animations;
        
        static const short kCurrentModelVersion;
    };
    
}
