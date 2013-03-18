#include "pixelboost/asset/model.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/resources/modelResource.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::ModelResource)

ModelResource::ModelResource()
{
    
}

ModelResource::~ModelResource()
{
    
}

ResourceError ModelResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
            if (!file)
            {
                PbLogError("pb.resource.xml", "Error opening XML file (%s)", filename.c_str());
                return kResourceErrorNoSuchResource;
            }
            
            LoadModel(file, _Model);
            
            return kResourceErrorNone;
        }
            
        case kResourceProcessProcess:
        case kResourceProcessPostProcess:
        {
            return kResourceErrorNone;
        }
            
        case kResourceProcessUnload:
        {
            _Model = ModelDefinition();
            return kResourceErrorNone;
        }
    }
}

const ModelDefinition& ModelResource::GetModelDefinition()
{
    return _Model;
}

bool ModelResource::LoadModel(File* file, ModelDefinition& model)
{
    short version;
    file->Read(version);
    
    if (version != ModelDefinition::kCurrentModelVersion)
    {
        delete file;
        PbLogError("pb.graphics.model", "Model version mismatch - expected %d, got %d", ModelDefinition::kCurrentModelVersion, version);
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
        if (!LoadMesh(file, o))
        {
            delete file;
            return false;
        }
        model.Meshes.push_back(o);
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
        if (!LoadBone(file, b))
        {
            delete file;
            return false;
        }
        model.Bones.push_back(b);
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
        if (!LoadAnimation(file, a))
        {
            delete file;
            return false;
        }
        model.Animations.push_back(a);
    }
    
    delete file;
    
    return true;
}

bool ModelResource::LoadMesh(File* file, ModelMeshDefinition& mesh)
{
    file->Read(mesh.Indexed);
    file->Read(mesh.Skinned);
    
    short vertexFormat;
    file->Read(vertexFormat);
    mesh.VertexFormat = (ModelVertexFormat)vertexFormat;
    
    short numVertices;
    file->Read(numVertices);
    
    float boundsSize;
    file->Read(boundsSize);
    mesh.Bounds.Set(glm::vec3(0,0,0), boundsSize);
    
    for (int i=0; i<numVertices; i++)
    {
        ModelVertex v;

        switch (mesh.VertexFormat)
        {
            case kModelVertexFormat_1P_1N_1UV:
            {
                file->Read(v.Position.x);
                file->Read(v.Position.y);
                file->Read(v.Position.z);
                file->Read(v.Normal.x);
                file->Read(v.Normal.y);
                file->Read(v.Normal.z);
                file->Read(v.UV.x);
                file->Read(v.UV.y);
                break;
            }
            case kModelVertexFormat_1P_1N_1UV_4BW:
            {
                file->Read(v.Position.x);
                file->Read(v.Position.y);
                file->Read(v.Position.z);
                file->Read(v.Normal.x);
                file->Read(v.Normal.y);
                file->Read(v.Normal.z);
                file->Read(v.UV.x);
                file->Read(v.UV.y);
                file->Read(v.Bone[0]);
                file->Read(v.Bone[1]);
                file->Read(v.Bone[2]);
                file->Read(v.Bone[3]);
                file->Read(v.BoneWeights[0]);
                file->Read(v.BoneWeights[1]);
                file->Read(v.BoneWeights[2]);
                file->Read(v.BoneWeights[3]);
                break;
            }
            default:
            {
                return false;
            }
        }
        
        mesh.Vertices.push_back(v);
    }
    
    if (mesh.Indexed)
    {
        short numIndices;
        if (!file->Read(numIndices))
            return false;
        
        for (int i=0; i<numIndices; i++)
        {
            short index;
            if (!file->Read(index))
                return false;
            mesh.Indices.push_back(index);
        }
    }
    
    return true;
}

bool ModelResource::LoadBone(File* file, ModelBoneDefinition& bone)
{
    int nameLength;
    file->Read(nameLength);
    char* name = new char[nameLength+1];
    file->Read((unsigned char*)name, nameLength+1);
    bone._Name = name;
    delete[] name;
    
    file->Read(bone._Id);
    file->Read(bone._ParentId);
    
    file->Read(bone._Position.x);
    file->Read(bone._Position.y);
    file->Read(bone._Position.z);
    
    file->Read(bone._Rotation.x);
    file->Read(bone._Rotation.y);
    file->Read(bone._Rotation.z);
    file->Read(bone._Rotation.w);
    
    for (int x=0; x<4; x++)
        for (int y=0; y<4; y++)
            file->Read(bone._BindMatrix[x][y]);
    
    return true;
}

bool ModelResource::LoadAnimation(File* file, ModelAnimationDefinition& animation)
{
    int nameLength;
    file->Read(nameLength);
    char* name = new char[nameLength+1];
    file->Read((unsigned char*)name, nameLength+1);
    animation._Name = name;
    delete[] name;
    
    file->Read(animation._FPS);
    file->Read(animation._Length);
    
    short numFrames;
    file->Read(numFrames);
    
    for (int frameIdx=0; frameIdx<numFrames; frameIdx++)
    {
        short numMatrices;
        file->Read(numMatrices);
        
        ModelAnimationDefinition::AnimationFrame frame;
        
        for (int matrixIdx=0; matrixIdx<numMatrices; matrixIdx++)
        {
            glm::mat4x4 matrix;
            
            for (int x=0; x<4; x++)
                for (int y=0; y<4; y++)
                    file->Read(matrix[x][y]);
            
            frame.push_back(matrix);
        }
        
        animation._Frames.push_back(frame);
    }
    
    return true;
}
