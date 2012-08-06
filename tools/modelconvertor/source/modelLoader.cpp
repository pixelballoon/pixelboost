#include "model.h"
#include "modelLoader.h"

ModelLoader::~ModelLoader()
{
    
}

ObjLoader::ObjLoader(const std::string& filename)
    : _Filename(filename)
    , _Model(0)
{
    
}

ObjLoader::~ObjLoader()
{
    delete _Model;
}

bool ObjLoader::Process()
{
    if (_Model)
        return true;
    
    _Model = new Model();
    
    return true;
    
    /*
    std::string objFilename = fileName;
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(location, fileName);
    
    if (!file)
        return false;
    
    std::string model;
    
    if (!file->ReadAll(model))
    {
        delete file;
        return false;
    }
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    std::vector<Vertex_NPXYZ_UV> verts;
    
    enum ReadMode
    {
        kReadModeVertex,
        kReadModeTexture,
        kReadModeNormal,
        kReadModeFace
    };
    
    ReadMode readMode;
    
    std::vector<std::string> lines;
    
    SplitString(model, '\n', lines);
    
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::string line = *it;
        
        std::vector<std::string> command = SplitLine(line);
        
        if (command.size() < 1)
            continue;
        
        if (command[0] == "v")
            readMode = kReadModeVertex;
        else if (command[0] == "vt")
            readMode = kReadModeTexture;
        else if (command[0] == "vn")
            readMode = kReadModeNormal;
        else if (command[0] == "f")
            readMode = kReadModeFace;
        else
            continue;
        
        switch (readMode)
        {
            case kReadModeVertex:
            {
                vertices.push_back(glm::vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
                break;
            }
            case kReadModeTexture:
            {
                uvs.push_back(glm::vec2(atof(command[1].c_str()), atof(command[2].c_str())));
                break;
            }
            case kReadModeNormal:
            {
                normals.push_back(glm::vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
                break;
            }
            case kReadModeFace:
            {
                // Only support triangulated faces at the moment
                if (command.size() == 4)
                {
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[2], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                } else {
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[2], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                    ParseVert(verts, command[1], vertices, uvs, normals);
                    ParseVert(verts, command[3], vertices, uvs, normals);
                    ParseVert(verts, command[4], vertices, uvs, normals);
                }
                break;
            }
        }
    }
    
    delete file;
    
    _NumVertices = verts.size();
    
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_NP_XYZ_UV, _NumVertices);
    _VertexBuffer->Lock();
    Vertex_NPXYZ_UV* vertexBuffer = static_cast<Vertex_NPXYZ_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<_NumVertices; i++)
    {
        vertexBuffer[i].position[0] = verts[i].position[0];
        vertexBuffer[i].position[1] = verts[i].position[1];
        vertexBuffer[i].position[2] = verts[i].position[2];
        vertexBuffer[i].uv[0] = verts[i].uv[0];
        vertexBuffer[i].uv[1] = verts[i].uv[1];
        vertexBuffer[i].normal[0] = verts[i].normal[0];
        vertexBuffer[i].normal[1] = verts[i].normal[1];
        vertexBuffer[i].normal[2] = verts[i].normal[2];
    }
    _VertexBuffer->Unlock();
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _NumVertices);
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_NumVertices; i++)
    {
        indexBuffer[i] = i;
    }
    _IndexBuffer->Unlock();
    */
}

const Model* ObjLoader::GetModel() const
{
    return _Model;
}
