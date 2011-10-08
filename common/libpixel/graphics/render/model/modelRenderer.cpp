#include <fstream>
#include <sstream>

#include <vector>

#include "libpixel/file/fileHelpers.h"
#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/render/model/modelRenderer.h"

namespace libpixel
{

Model::Model()
    : _RefCount(1)
{
    
}

Model::~Model()
{
    // TODO: Release buffers
}
    
bool Model::Load(const std::string& modelName)
{
    std::string objFilename = FileHelpers::GetRootPath() + "/data/models/" + modelName + ".obj";
    
    std::fstream file;
    
    file.open(objFilename.c_str(), std::ios_base::in);
    
    if (!file.is_open())
        return false;
    
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    
    std::vector<Vertex> verts;
    
    enum ReadMode
    {
        kReadModeVertex,
        kReadModeTexture,
        kReadModeNormal,
        kReadModeFace
    };
    
    ReadMode readMode;
    
    char lineChars[1024];
    while (!file.eof())
    {
        file.getline(lineChars, 1024);
        
        std::string line = lineChars;
        
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
                vertices.push_back(Vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
                break;
            }
            case kReadModeTexture:
            {
                uvs.push_back(Vec2(atof(command[1].c_str()), atof(command[2].c_str())));
                break;
            }
            case kReadModeNormal:
            {
                normals.push_back(Vec3(atof(command[1].c_str()), atof(command[2].c_str()), atof(command[3].c_str())));
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
                }
                break;
            }
        }
    }

    file.close();
    
    _NumVertices = verts.size();

	glGenBuffers(1, &_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _NumVertices, 0, GL_STATIC_DRAW);
	Vertex* vertexBuffer = (Vertex*)glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    for (int i=0; i<_NumVertices; i++)
    {
        vertexBuffer[i]._Position[0] = verts[i]._Position[0];
        vertexBuffer[i]._Position[1] = verts[i]._Position[1];
        vertexBuffer[i]._Position[2] = verts[i]._Position[2];
        vertexBuffer[i]._UV[0] = verts[i]._UV[0];
        vertexBuffer[i]._UV[1] = verts[i]._UV[1];
        vertexBuffer[i]._Normal[0] = verts[i]._Normal[0];
        vertexBuffer[i]._Normal[1] = verts[i]._Normal[1];
        vertexBuffer[i]._Normal[2] = verts[i]._Normal[2];
    }
	glUnmapBufferOES(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glGenBuffers(1, &_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * _NumVertices, 0, GL_STATIC_DRAW);
    GLushort* indexBuffer = (GLushort*)glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
    for (int i=0; i<_NumVertices; i++)
    {
        indexBuffer[i] = i;
    }
    glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
	return true;
}
    
void Model::ParseVert(std::vector<Vertex>& verts, const std::string& vert, const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs, const std::vector<Vec3>& normals)
{
    std::vector<std::string> vertIndices = SplitPath(vert);
    
    if (vertIndices.size() < 3)
        return;
    
    int posIndex = atoi(vertIndices[0].c_str());
    int uvIndex = atoi(vertIndices[1].c_str());
    int normalIndex = atoi(vertIndices[2].c_str());
    
    Vec3 pos = vertices[posIndex-1];
    Vec2 uv = uvs[uvIndex-1];
    Vec3 normal = normals[normalIndex-1];
    
    Vertex vertex;
    vertex._Position[0] = pos[0];
    vertex._Position[1] = pos[1];
    vertex._Position[2] = pos[2];
    vertex._UV[0] = uv[0];
    vertex._UV[1] = 1.f-uv[1];
    vertex._Normal[0] = normal[0];
    vertex._Normal[1] = normal[1];
    vertex._Normal[2] = normal[2];
    
    verts.push_back(vertex);
}
    
std::vector<std::string>& Model::SplitString(const std::string &string, char delim, std::vector<std::string> &items)
{
    std::stringstream stream(string);
    std::string item;
    while(std::getline(stream, item, delim)) {
        items.push_back(item);
    }
    return items;
}

std::vector<std::string> Model::SplitLine(const std::string &string)
{
    std::vector<std::string> items;
    return SplitString(string, ' ', items);
}


std::vector<std::string> Model::SplitPath(const std::string &string)
{
    std::vector<std::string> items;
    return SplitString(string, '/', items);
}
    
Texture::Texture()
{
    
}

Texture::~Texture()
{
    // TODO: Release texture
}

bool Texture::Load(const std::string& textureName)
{
    std::string texFilename = FileHelpers::GetRootPath() + "/data/models/" + textureName + ".png";
    
    _Texture = LoadTexture(texFilename, true);
    
    return true;
}

ModelRenderer::ModelRenderer()
{
    
}

ModelRenderer::~ModelRenderer()
{
    for (ModelMap::iterator it = _Models.begin(); it != _Models.end(); ++it)
    {
        delete it->second;
    }
}
    
bool ModelRenderer::LoadModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it != _Models.end())
    {
        it->second->_RefCount++;
        return true;
    }
    
    Model* model = new Model();
    model->Load(modelName);
    
    _Models[modelName] = model;
    
    return false;
}

bool ModelRenderer::UnloadModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it == _Models.end())
        return false;
    
    it->second->_RefCount--;
    
    if (it->second->_RefCount == 0)
    {
        delete it->second;
        _Models.erase(it);
    }
    
    return true;
}
    
bool ModelRenderer::LoadTexture(const std::string& textureName)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it != _Textures.end())
    {
        it->second->_RefCount++;
        return true;
    }
    
    Texture* texture = new Texture();
    texture->Load(textureName);
    
    _Textures[textureName] = texture;
    
    return false;
}

bool ModelRenderer::UnloadTexture(const std::string& textureName)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it == _Textures.end())
        return false;
    
    it->second->_RefCount--;
    
    if (it->second->_RefCount == 0)
    {
        delete it->second;
        _Textures.erase(it);
    }
    
    return true;
}
    
bool ModelRenderer::Render(const std::string& modelName, const std::string& textureName, Vec3 position, Vec3 rotation, Vec3 scale, Vec3 offset)
{
    Model* model = GetModel(modelName);
    Texture* texture = GetTexture(textureName);
    
    if (!model || !texture)
        return false;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glPushMatrix();
    
    glTranslatef(position[0], position[1], position[2]);
    glScalef(scale[0], scale[1], scale[2]);
    glTranslatef(offset[0], offset[1], offset[2]);
    glRotatef(rotation[0], 1, 0, 0);
    glRotatef(rotation[1], 0, 1, 0);
    glRotatef(rotation[2], 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, texture->_Texture);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, model->_VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->_IndexBuffer);
    
    GLsizei stride = sizeof(Vertex);
    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex, _Position));
    glNormalPointer(GL_FLOAT, stride, (void*)offsetof(Vertex, _Normal));
    glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex, _UV));
    
    glDrawElements(GL_TRIANGLES, model->_NumVertices, GL_UNSIGNED_SHORT, (void*)0);
    
    glPopMatrix();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    return true;
}
    
Model* ModelRenderer::GetModel(const std::string& modelName)
{
    ModelMap::iterator it = _Models.find(modelName);
    
    if (it == _Models.end())
        return 0;

    return it->second;
}
    
Texture* ModelRenderer::GetTexture(const std::string& textureName)
{
    TextureMap::iterator it = _Textures.find(textureName);
    
    if (it == _Textures.end())
        return 0;
    
    return it->second;   
}

}
