#include <sstream>
#include <string>

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/renderer/model/model.h"

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
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(pb::kFileLocationUser, _Filename);
    
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
    
    std::vector<pb::Vertex_NPXYZ_UV> verts;
    
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
        
        std::vector<std::string> command;
        SplitString(line, ' ', command);
        
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
     
    _Model = new pb::ModelDefinition();
    
    pb::ModelObject object;
    
    object.Indexed = false;
    object.VertexFormat = pb::kVertexFormat_NP_XYZ_UV;
    
    for (int i=0; i<verts.size(); i++)
    {
        pb::ModelVertex vertex;
        vertex.Position = glm::vec3(verts[i].position[0], verts[i].position[1], verts[i].position[2]);
        vertex.Normal = glm::vec3(verts[i].normal[0], verts[i].normal[1], verts[i].normal[2]);
        vertex.UV = glm::vec2(verts[i].uv[0], verts[i].uv[1]);
        object.Vertices.push_back(vertex);
    }
    
    _Model->Objects.push_back(object);
    
    return true;
}

const pb::ModelDefinition* ObjLoader::GetModel() const
{
    return _Model;
}

pb::ModelDefinition* ObjLoader::GetModel()
{
    return _Model;
}

void ObjLoader::ParseVert(std::vector<pb::Vertex_NPXYZ_UV>& verts, const std::string& vert, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals)
{
    std::vector<std::string> vertIndices;
    SplitString(vert, '/', vertIndices);
    
    if (vertIndices.size() < 2)
        return;
    
    pb::Vertex_NPXYZ_UV vertex;
    
    int posIndex = atoi(vertIndices[0].c_str());
    glm::vec3 pos = vertices[posIndex-1];
    vertex.position[0] = pos[0];
    vertex.position[1] = pos[1];
    vertex.position[2] = pos[2];
    
    int uvIndex = atoi(vertIndices[1].c_str());
    glm::vec2 uv = uvs[uvIndex-1];
    vertex.uv[0] = uv[0];
    vertex.uv[1] = 1.f-uv[1];
    
    if (vertIndices.size() > 2)
    {
        int normalIndex = atoi(vertIndices[2].c_str());
        glm::vec3 normal = normals[normalIndex-1];
        vertex.normal[0] = normal[0];
        vertex.normal[1] = normal[1];
        vertex.normal[2] = normal[2];
    }
    
    verts.push_back(vertex);
}

std::vector<std::string>& ObjLoader::SplitString(const std::string &string, char delim, std::vector<std::string> &items)
{
    std::stringstream stream(string);
    std::string item;
    while(std::getline(stream, item, delim)) {
        items.push_back(item);
    }
    return items;
}
