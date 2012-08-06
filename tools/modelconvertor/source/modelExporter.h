#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

class ModelLoader;

class ModelExporter
{
public:
    ModelExporter(ModelLoader* modelLoader);
    virtual ~ModelExporter();
    
    bool Save(const std::string& filename);
    bool Process();
    
private:
    ModelLoader* _ModelLoader;
    std::vector<glm::vec3> _Vertices;
};
