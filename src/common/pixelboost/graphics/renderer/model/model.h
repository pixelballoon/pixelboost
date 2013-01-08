#pragma once

#include <vector>

#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/bufferFormats.h"
#include "pixelboost/maths/boundingSphere.h"

namespace pb
{

    class ModelVertex
    {
    public:
        ModelVertex();
        
        bool Read(pb::VertexFormat vertexFormat, pb::File* file);
        bool Write(pb::VertexFormat vertexFormat, pb::File* file) const;
        
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
        
        bool Read(pb::File* file);
        bool Write(pb::File* file) const;
        
        void CalculateBounds();
        
    public:
        bool Skinned;
        bool Indexed;
        pb::BoundingSphere Bounds;
        pb::VertexFormat VertexFormat;
        
        std::vector<short> Indices;
        std::vector<ModelVertex> Vertices;
    };
    
    class ModelBoneDefinition
    {
    public:
        ModelBoneDefinition();
        
        bool Read(pb::File* file);
        bool Write(pb::File* file) const;
    
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
        
        bool Read(pb::File* file);
        bool Write(pb::File* file) const;

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
        
        bool Open(pb::FileLocation fileLocation, const std::string& filename);
        bool Save(pb::FileLocation fileLocation, const std::string& filename) const;
        
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
