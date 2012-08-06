#pragma once

#include <vector>

#include "glm/glm.hpp"

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