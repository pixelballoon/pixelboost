#pragma once

#include "glm/glm.hpp"

#include "pixelboost/framework/definitions.h"

namespace pb
{

    class RenderLayer;
    class Renderable;
    class ShaderPass;
    class Viewport;
        
    class IRenderer
    {
    public:
        IRenderer();
        virtual ~IRenderer();
        
        virtual void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix) = 0;
    };

}
