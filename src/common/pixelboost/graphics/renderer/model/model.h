#pragma once

#include <vector>

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
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Color;
        glm::vec2 UV;
    };

    class ModelObject
    {
    public:
        ModelObject();
        
        bool Read(pb::File* file);
        bool Write(pb::File* file) const;
        
        void CalculateBounds();
        
    public:
        bool Indexed;
        pb::BoundingSphere Bounds;
        pb::VertexFormat VertexFormat;
        
        std::vector<short> Indices;
        std::vector<ModelVertex> Vertices;
    };

    class ModelDefinition
    {
    public:
        ModelDefinition();
        
        bool Open(pb::FileLocation fileLocation, const std::string& filename);
        bool Save(pb::FileLocation fileLocation, const std::string& filename) const;
        
        void CalculateBounds();
        
    public:
        std::vector<ModelObject> Objects;
        
    private:
        short _Version;
        
        static const short kCurrentModelVersion;
    };
    
}
