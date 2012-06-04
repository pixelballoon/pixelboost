#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    
class Effect;
class IndexBuffer;
class VertexBuffer;
class Viewport;
    
class PrimitiveRenderable : public Renderable
{
public:
    virtual Uid GetRenderableType();
    
    enum Type
    {
        kTypeEllipse,
        kTypeLine,
        kTypeRectangle,
    };
    
    virtual Type GetPrimitiveType() = 0;
    
public:
    glm::vec4 Color;
};
    
class PrimitiveRenderableEllipse : public PrimitiveRenderable
{
public:
    virtual Type GetPrimitiveType();
    
public:
    bool Solid;
    
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec2 Size;
};

class PrimitiveRenderableLine : public PrimitiveRenderable
{
public:
    virtual Type GetPrimitiveType();
    
    glm::vec3 Start;
    glm::vec3 End;
};

    
class PrimitiveRenderableRectangle : public PrimitiveRenderable
{
public:
    virtual Type GetPrimitiveType();
    
public:
    bool Solid;
    
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec2 Size;
};
    
class PrimitiveRenderer : public IRenderer
{
public:
    PrimitiveRenderer();
    ~PrimitiveRenderer();
    
    void Update(float time);
    void Render(int count, Renderable* renderables, Viewport* viewport);
    
private:
    Effect* _Effect;
    
    IndexBuffer* _EllipseIndexBuffer;
    VertexBuffer* _EllipseVertexBuffer;
    
    IndexBuffer* _LineIndexBuffer;
    VertexBuffer* _LineVertexBuffer;
    
    IndexBuffer* _BoxIndexBuffer;
    VertexBuffer* _BoxVertexBuffer;
};
    
}
