#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/db/struct.h"

namespace pb
{
    
class Record;
    
class Entity : public Struct
{
public:
    Entity(Uid uid, Uid type, void* data);
    virtual ~Entity();
    
    void Load();
    
    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    
private:
    glm::vec3 _Position;
    glm::vec3 _Rotation;
    glm::vec3 _Scale;
};
    
}
