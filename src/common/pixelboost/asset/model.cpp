#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/asset/model.h"

using namespace pb;

const short ModelDefinition::kCurrentModelVersion = 3;

ModelVertex::ModelVertex()
{
    
}
    
bool ModelVertex::Read(ModelVertexFormat vertexFormat, File* file)
{
    switch (vertexFormat)
    {
        case kModelVertexFormat_1P_1N_1UV:
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
        case kModelVertexFormat_1P_1N_1UV_4BW:
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

bool ModelVertex::Write(ModelVertexFormat vertexFormat, File* file) const
{
    switch (vertexFormat)
    {
        case kModelVertexFormat_1P_1N_1UV:
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
        case kModelVertexFormat_1P_1N_1UV_4BW:
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
    
ModelMeshDefinition::ModelMeshDefinition()
    : Indexed(false)
    , Skinned(false)
{
    
}
    
bool ModelMeshDefinition::Read(pb::File* file)
{
    file->Read(Indexed);
    file->Read(Skinned);
    
    short vertexFormat;
    file->Read(vertexFormat);
    VertexFormat = (ModelVertexFormat)vertexFormat;
    
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

bool ModelMeshDefinition::Write(pb::File* file) const
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

void ModelMeshDefinition::CalculateBounds()
{
    float maxBounds = 0;
    for (std::vector<ModelVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++it)
    {
        maxBounds = glm::max(maxBounds, glm::length(it->Position));
    }
    Bounds.Set(glm::vec3(0,0,0), maxBounds);
}

ModelBoneDefinition::ModelBoneDefinition()
{
    
}

bool ModelBoneDefinition::Read(pb::File* file)
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

bool ModelBoneDefinition::Write(pb::File* file) const
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

ModelAnimationDefinition::ModelAnimationDefinition()
{
    
}

bool ModelAnimationDefinition::Read(pb::File* file)
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

bool ModelAnimationDefinition::Write(pb::File* file) const
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
    
bool ModelDefinition::Open(const std::string& filename)
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
    
    if (!file)
    {
        PbLogError("pb.graphics.model", "Unable to load model %s", filename.c_str());
        return false;
    }
    
    file->Read(_Version);
    
    if (_Version != kCurrentModelVersion)
    {
        delete file;
        PbLogError("pb.graphics.model", "Model (%s) version mismatch - expected %d, got %d", filename.c_str(), kCurrentModelVersion, _Version);
        return false;
    }
    
    short numMeshes;
    if (!file->Read(numMeshes))
    {
        delete file;
        return false;
    }
    
    for (int i=0; i<numMeshes; i++)
    {
        ModelMeshDefinition o;
        if (!o.Read(file))
        {
            delete file;
            return false;
        }
        Meshes.push_back(o);
    }
    
    short numBones;
    if (!file->Read(numBones))
    {
        delete file;
        return false;
    }
    
    for (int i=0; i<numBones; i++)
    {
        ModelBoneDefinition b;
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
        ModelAnimationDefinition a;
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

bool ModelDefinition::Save(const std::string& filename) const
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile(filename, pb::kFileModeWrite);
    
    if (!file)
        return false;
    
    file->Write(_Version);
    
    file->Write(static_cast<short>(Meshes.size()));
    
    for (std::vector<ModelMeshDefinition>::const_iterator it = Meshes.begin(); it != Meshes.end(); ++it)
    {
        if (!it->Write(file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(Bones.size()));
    
    for (std::vector<ModelBoneDefinition>::const_iterator it = Bones.begin(); it != Bones.end(); ++it)
    {
        if (!it->Write(file))
        {
            delete file;
            return false;
        }
    }
    
    file->Write(static_cast<short>(Animations.size()));
    
    for (std::vector<ModelAnimationDefinition>::const_iterator it = Animations.begin(); it != Animations.end(); ++it)
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
    for (std::vector<ModelMeshDefinition>::iterator it = Meshes.begin(); it != Meshes.end(); ++it)
    {
        it->CalculateBounds();
    }
}
