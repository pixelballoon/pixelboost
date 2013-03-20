#pragma once

#include "glm/glm.hpp"

#include "pixelboost/input/inputManager.h"

namespace pb
{
    class AccelerometerHandler : public virtual InputHandler
    {
    public:
        AccelerometerHandler();
        virtual ~AccelerometerHandler();
        
        virtual bool OnAccelerometer(glm::vec3 attitude, glm::vec3 rotationRate, glm::vec3 gravity, glm::vec3 userAcceleration);
    };
    
    class AccelerometerManager : public InputManager
    {
    public:
        AccelerometerManager();
        ~AccelerometerManager();
        
        void OnAccelerometer(glm::vec3 attitude, glm::vec3 rotationRate, glm::vec3 gravity, glm::vec3 userAcceleration);
    };
    
}
