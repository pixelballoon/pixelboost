#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/data/json/writer.h"
#include "pixelboost/graphics/device/bufferFormats.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec4 Color;
    glm::vec2 UV;
};

struct ModelObject
{
    pb::VertexFormat VertexFormat;
    std::vector<Vertex> Vertices;
};

struct Model
{
    std::vector<ModelObject> Models;
};

class ModelLoader
{
public:
    virtual ~ModelLoader();
    
    virtual void Process() = 0;
    virtual const Model& GetModel() const = 0;
};

class ObjLoader : public ModelLoader
{
public:
    ObjLoader(const std::string& filename);
    virtual ~ObjLoader();
    
    virtual void Process();
    virtual const Model& GetModel() const;
    
private:
    std::string _Filename;
    Model _Model;
};

class ModelConvertor
{
public:
    ModelConvertor(ModelLoader* modelLoader);
    virtual ~ModelConvertor();
    
    bool Save(const std::string& filename);
    bool Process();
    
private:
    ModelLoader* _ModelLoader;
    std::vector<glm::vec3> _Vertices;
};

int main (int argc, const char * argv[])
{
    const char* inputLocation = argc > 1 ? argv[1] : "";
    
    const char* outputLocation = argc > 2 ? argv[2] : "";
    
    ModelConvertor modelConvertor(new ObjLoader(inputLocation));
    modelConvertor.Process();
    modelConvertor.Save(outputLocation);

    return 0;
}

ModelLoader::~ModelLoader()
{
    
}

ObjLoader::~ObjLoader()
{
    
}

ObjLoader::ObjLoader(const std::string& filename)
    : _Filename(filename)
{
    
}

void ObjLoader::Process()
{
    
}

const Model& ObjLoader::GetModel() const
{
    return _Model;
}

ModelConvertor::ModelConvertor(ModelLoader* modelLoader)
    : _ModelLoader(modelLoader)
{
    
}

ModelConvertor::~ModelConvertor()
{
    delete _ModelLoader;
}

bool ModelConvertor::Save(const std::string& filename)
{
    return false;
}

bool ModelConvertor::Process()
{
    return false;
}
