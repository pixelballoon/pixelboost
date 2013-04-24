#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/db/struct.h"

namespace pb
{
    
    class DbEntity : public DbStruct
    {
    public:
        DbEntity(Uid uid, Uid type, DbStructData* data);
        virtual ~DbEntity();
        
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
