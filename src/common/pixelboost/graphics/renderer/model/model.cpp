#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/renderer/model/model.h"

using namespace pb;

const short ModelDefinition::kCurrentModelVersion = 1;

ModelVertex::ModelVertex()
{
    
}
    
bool ModelVertex::Read(pb::VertexFormat vertexFormat, pb::File* file)
{
    switch (vertexFormat)
    {
        case pb::kVertexFormat_NP_XYZ_UV:
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
        case pb::kVertexFormat_NP_XYZ_UV:
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
        default:
        {
            return false;
        }
    }

    return true;
}
    
ModelObject::ModelObject()
{
    
}
    
bool ModelObject::Read(pb::File* file)
{
    file->Read(Indexed);
    
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
