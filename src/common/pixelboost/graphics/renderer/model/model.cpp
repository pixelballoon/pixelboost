#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/renderer/model/model.h"

using namespace pb;

const short ModelDefinition::kCurrentModelVersion = 2;

ModelVertex::ModelVertex()
{
    
}
    
bool ModelVertex::Read(pb::VertexFormat vertexFormat, pb::File* file)
{
    switch (vertexFormat)
    {
        case pb::kVertexFormat_P3_N3_UV:
        {
            file->Read(Position.x);
            file->Read(Position.y);
            file->Read(Position.z);
            file->Read(Normal.x);
            file->Read(Normal.y);
            file->Read(Normal.z);
            file->Read(UV.x);
            file->Read(UV.y);
            break;
        }
        case pb::kVertexFormat_P3_N3_UV_BW:
        {
            file->Read(Position.x);
            file->Read(Position.y);
            file->Read(Position.z);
            file->Read(Normal.x);
            file->Read(Normal.y);
            file->Read(Normal.z);
            file->Read(UV.x);
            file->Read(UV.y);
            file->Read(Bone[0]);
            file->Read(Bone[1]);
            file->Read(Bone[2]);
            file->Read(Bone[3]);
            file->Read(BoneWeights[0]);
            file->Read(BoneWeights[1]);
            file->Read(BoneWeights[2]);
            file->Read(BoneWeights[3]);
            break;
        }
        default:
        {
            return false;
        }
    }
    
    return true;
}

bool ModelVertex::Write(pb::VertexFormat vertexFormat, pb::File* file) const
{
    switch (vertexFormat)
    {
        case pb::kVertexFormat_P3_N3_UV:
        {
            file->Write(Position.x);
            file->Write(Position.y);
            file->Write(Position.z);
            file->Write(Normal.x);
            file->Write(Normal.y);
            file->Write(Normal.z);
            file->Write(UV.x);
            file->Write(UV.y);
            break;
        }
        case pb::kVertexFormat_P3_N3_UV_BW:
        {
            file->Write(Position.x);
            file->Write(Position.y);
            file->Write(Position.z);
            file->Write(Normal.x);
            file->Write(Normal.y);
            file->Write(Normal.z);
            file->Write(UV.x);
            file->Write(UV.y);
            file->Write(Bone[0]);
            file->Write(Bone[1]);
            file->Write(Bone[2]);
            file->Write(Bone[3]);
            file->Write(BoneWeights[0]);
            file->Write(BoneWeights[1]);
            file->Write(BoneWeights[2]);
            file->Write(BoneWeights[3]);
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}
    
ModelObject::ModelObject()
    : Indexed(false)
    , Skinned(false)
{
    
}
    
bool ModelObject::Read(pb::File* file)
{
    file->Read(Indexed);
    file->Read(Skinned);
    
    short vertexFormat;
    file->Read(vertexFormat);
    VertexFormat = (pb::VertexFormat)vertexFormat;
    
    short numVertices;
    file->Read(numVertices);
    
    float boundsSize;
    file->Read(boundsSize);
    Bounds.Set(glm::vec3(0,0,0), boundsSize);
    
    for (int i=0; i<numVertices; i++)
    {
        ModelVertex v;
        if (!v.Read(VertexFormat, file))
            return false;
        Vertices.push_back(v);
    }
    
    if (Indexed)
    {
        short numIndices;
        if (!file->Read(numIndices))
            return false;
        
        for (int i=0; i<numIndices; i++)
        {
            short index;
            if (!file->Read(index))
                return false;
            Indices.push_back(index);
        }
    }
    
    return true;
}

bool ModelObject::Write(pb::File* file) const
{
    file->Write(Indexed);
    file->Write(Skinned);
    file->Write(static_cast<short>(VertexFormat));
    file->Write(static_cast<short>(Vertices.size()));
    
    file->Write(Bounds.GetSize());
    
    for (std::vector<ModelVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        if (!it->Write(VertexFormat, file))
            return false;
    }
    
    if (Indexed)
    {
        file->Write(static_cast<short>(Indices.size()));
        
        for (std::vector<short>::const_iterator it = Indices.begin(); it != Indices.end(); ++it)
        {
            file->Write(*it);
        }
    }
    
    return true;
}

void ModelObject::CalculateBounds()
{
    float maxBounds = 0;
    for (std::vector<ModelVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        maxBounds = glm::max(maxBounds, glm::length(it->Position));
    }
    Bounds.Set(glm::vec3(0,0,0), maxBounds);
}

ModelBone::ModelBone()
{
    
}

bool ModelBone::Read(pb::File* file)
{
    int nameLength;
    file->Read(nameLength);
    char* name = new char[nameLength+1];
    file->Read((unsigned char*)name, nameLength+1);
    _Name = name;
    delete[] name;
    
    file->Read(_Id);
    file->Read(_ParentId);

    file->Read(_Position.x);
    file->Read(_Position.y);
    file->Read(_Position.z);
    
    file->Read(_Rotation.x);
    file->Read(_Rotation.y);
    file->Read(_Rotation.z);
    file->Read(_Rotation.w);
    
    for (int x=0; x<4; x++)
        for (int y=0; y<4; y++)
            file->Read(_BindMatrix[x][y]);
    
    return true;
}

bool ModelBone::Write(pb::File* file) const
{
    file->Write(static_cast<int>(_Name.length()));
    file->Write((unsigned char*)_Name.c_str(), _Name.length()+1);
    
    file->Write(_Id);
    file->Write(_ParentId);
   
    file->Write(_Position.x);
    file->Write(_Position.y);
    file->Write(_Position.z);
    
    file->Write(_Rotation.x);
    file->Write(_Rotation.y);
    file->Write(_Rotation.z);
    file->Write(_Rotation.w);

    for (int x=0; x<4; x++)
        for (int y=0; y<4; y++)
            file->Write(_BindMatrix[x][y]);
    
    return true;
}

ModelAnimation::ModelAnimation()
{
    
}

bool ModelAnimation::Read(pb::File* file)
{
    int nameLength;
    file->Read(nameLength);
    char* name = new char[nameLength+1];
    file->Read((unsigned char*)name, nameLength+1);
    _Name = name;
    delete[] name;
    
    file->Read(_FPS);
    file->Read(_Length);
    
    short numFrames;
    file->Read(numFrames);
    
    for (int frameIdx=0; frameIdx<numFrames; frameIdx++)
    {
        short numMatrices;
        file->Read(numMatrices);
        
        AnimationFrame frame;
        
        for (int matrixIdx=0; matrixIdx<numMatrices; matrixIdx++)
        {
            glm::mat4x4 matrix;
            
            for (int x=0; x<4; x++)
                for (int y=0; y<4; y++)
                    file->Read(matrix[x][y]);
            
            frame.push_back(matrix);
        }
        
        _Frames.push_back(frame);
    }
    
    return true;
}

bool ModelAnimation::Write(pb::File* file) const
{
    file->Write(static_cast<int>(_Name.length()));
    file->Write((unsigned char*)_Name.c_str(), _Name.length()+1);
    
    file->Write(_FPS);
    file->Write(_Length);
    
    file->Write(static_cast<short>(_Frames.size()));
    for (std::vector<AnimationFrame>::const_iterator frameIt = _Frames.begin(); frameIt != _Frames.end(); ++frameIt)
    {
        file->Write(static_cast<short>(frameIt->size()));
        for (std::vector<glm::mat4x4>::const_iterator matrixIt = frameIt->begin(); matrixIt != frameIt->end(); ++matrixIt)
        {
            for (int x=0; x<4; x++)
                for (int y=0; y<4; y++)
                    file->Write((*matrixIt)[x][y]);
        }
    }
    
    return true;
}

ModelDefinition::ModelDefinition()
    : _Version(kCurrentModelVersion)
{
    
}
    
bool ModelDefinition::Open(pb::FileLocation fileLocation, const std::string& filename)
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(fileLocation, filename, pb::kFileModeReadOnly);
    
    if (!file)
    {
        PbLogError("pb.assets", "Unable to load model %s", filename.c_str());
        return false;
    }
    
    file->Read(_Version);
    
    if (_Version != kCurrentModelVersion)
    {
        delete file;
        PbLogError("pb.assets", "Model (%s) version mismatch - expected %d, got %d", filename.c_str(), kCurrentModelVersion, _Version);
        return false;
    }
    
    short numObjects;
    if (!file->Read(numObjects))
    {
        delete file;
        return false;
    }
    
    for (int i=0; i<numObjects; i++)
    {
        ModelObject o;
        if (!o.Read(file))
        {
            delete file;
            return false;
        }
        Objects.push_back(o);
    }
    
    short numBones;
    if (!file->Read(numBones))
    {
        delete file;
        return false;
    }
    
    for (int i=0; i<numBones; i++)
    {
        ModelBone b;
        if (!b.Read(file))
        {
            delete file;
            return false;
        }
        Bones.push_back(b);
    }
    
    short numAnimations;
    if (!file->Read(numAnimations))
    {
        delete file;
        return false;
    }
    
    for (int i=0; i<numAnimations; i++)
    {
        ModelAnimation a;
        if (!a.Read(file))
        {
            delete file;
            return false;
        }
        Animations.push_back(a);
    }
    
    delete file;
    
    return true;
}

bool ModelDefinition::Save(pb::FileLocation fileLocation, const std::string& filename) const
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(fileLocation, filename, pb::kFileModeWriteOnly);
    
    if (!file)
        return false;
    
    file->Write(_Version);
    
    file->Write(static_cast<short>(Objects.size()));
    
    for (std::vector<ModelObject>::const_iterator it = Objects.begin(); it != Objects.end(); ++it)
    {
        if (!it->Write(file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(Bones.size()));
    
    for (std::vector<ModelBone>::const_iterator it = Bones.begin(); it != Bones.end(); ++it)
    {
        if (!it->Write(file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(Animations.size()));
    
    for (std::vector<ModelAnimation>::const_iterator it = Animations.begin(); it != Animations.end(); ++it)
    {
        if (!it->Write(file))
        {
            delete file;
            return false;
        }
    }
    
    delete file;
    
    return true;
}

void ModelDefinition::CalculateBounds()
{
    for (std::vector<ModelObject>::iterator it = Objects.begin(); it != Objects.end(); ++it)
    {
        it->CalculateBounds();
    }
}
