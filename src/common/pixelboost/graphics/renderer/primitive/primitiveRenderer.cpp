#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

PrimitiveRenderer* PrimitiveRenderer::_Instance = 0;

PrimitiveRenderable::PrimitiveRenderable(Uid entityUid)
    : Renderable(entityUid)
    , _Color(glm::vec4(1,1,1,1))
{
    
}


Uid PrimitiveRenderable::GetType()
{
    return PrimitiveRenderable::GetStaticType();
}

Uid PrimitiveRenderable::GetStaticType()
{
    return TypeHash("pb::PrimitiveRenderable");
}

Shader* PrimitiveRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/shaders/pb_solid.shc");
}

void PrimitiveRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

glm::vec4 PrimitiveRenderable::GetColor()
{
    return _Color;
}

void PrimitiveRenderable::SetColor(glm::vec4 color)
{
    _Color = color;
}

PrimitiveRenderableEllipse::PrimitiveRenderableEllipse(Uid entityUid)
    : PrimitiveRenderable(entityUid)
    , _Solid(false)
{
    
}

void PrimitiveRenderableEllipse::CalculateBounds()
{
    BoundingSphere bounds;
    glm::vec4 position = GetWorldMatrix() * glm::vec4(0,0,0,1);
    bounds.Set(glm::vec3(position.x, position.y, position.z), glm::max(_Size.x, _Size.y));
    SetBounds(bounds);
}

void PrimitiveRenderableEllipse::CalculateWorldMatrix()
{
    glm::mat4x4 worldMatrix = glm::translate(glm::mat4x4(), _Position);
    worldMatrix = glm::scale(worldMatrix, glm::vec3(_Size, 1));
    worldMatrix = glm::rotate(worldMatrix, _Rotation[0], glm::vec3(1,0,0));
    worldMatrix = glm::rotate(worldMatrix, _Rotation[1], glm::vec3(0,1,0));
    worldMatrix = glm::rotate(worldMatrix, _Rotation[2], glm::vec3(0,0,1));
    worldMatrix = _Transform * worldMatrix;
    SetWorldMatrix(worldMatrix);
}

PrimitiveRenderable::Type PrimitiveRenderableEllipse::GetPrimitiveType()
{
    return kTypeEllipse;
}

bool PrimitiveRenderableEllipse::GetSolid()
{
    return _Solid;
}

void PrimitiveRenderableEllipse::SetSolid(bool solid)
{
    _Solid = solid;
}

glm::vec3 PrimitiveRenderableEllipse::GetPosition()
{
    return _Position;
}

void PrimitiveRenderableEllipse::SetPosition(glm::vec3 position)
{
    _Position = position;
    DirtyBounds();
    DirtyWorldMatrix();
}

glm::vec3 PrimitiveRenderableEllipse::GetRotation()
{
    return _Rotation;
}

void PrimitiveRenderableEllipse::SetRotation(glm::vec3 rotation)
{
    _Rotation = rotation;
    DirtyWorldMatrix();
}

glm::vec2 PrimitiveRenderableEllipse::GetSize()
{
    return _Size;
}

void PrimitiveRenderableEllipse::SetSize(glm::vec2 size)
{
    _Size = size;
    DirtyBounds();
    DirtyWorldMatrix();
}

PrimitiveRenderableLine::PrimitiveRenderableLine(Uid entityUid)
    : PrimitiveRenderable(entityUid)
{
    
}

void PrimitiveRenderableLine::CalculateBounds()
{
    BoundingSphere bounds((_Start+_End)/2.f, glm::distance(_Start, _End)/2.f);
    SetBounds(bounds);
}

void PrimitiveRenderableLine::CalculateWorldMatrix()
{
    SetWorldMatrix(_Transform);
}

PrimitiveRenderable::Type PrimitiveRenderableLine::GetPrimitiveType()
{
    return kTypeLine;
}

glm::vec3 PrimitiveRenderableLine::GetStart()
{
    return _Start;
}

void PrimitiveRenderableLine::SetStart(glm::vec3 start)
{
    _Start = start;
    DirtyBounds();
}

glm::vec3 PrimitiveRenderableLine::GetEnd()
{
    return _End;
}

void PrimitiveRenderableLine::SetEnd(glm::vec3 end)
{
    _End = end;
    DirtyBounds();
}

PrimitiveRenderableRectangle::PrimitiveRenderableRectangle(Uid entityUid)
    : PrimitiveRenderable(entityUid)
    , _Solid(false)
{
    
}

void PrimitiveRenderableRectangle::CalculateBounds()
{
    glm::vec4 position = GetWorldMatrix()[3];
    BoundingSphere bounds(glm::vec3(position.x, position.y, position.z), glm::max(_Size.x, _Size.y));
    SetBounds(bounds);
}

void PrimitiveRenderableRectangle::CalculateWorldMatrix()
{
    glm::mat4x4 worldMatrix = glm::scale(glm::mat4x4(), glm::vec3(_Size, 1));
    worldMatrix = _Transform * worldMatrix;
    SetWorldMatrix(worldMatrix);
}

PrimitiveRenderable::Type PrimitiveRenderableRectangle::GetPrimitiveType()
{
    return kTypeRectangle;
}

bool PrimitiveRenderableRectangle::GetSolid()
{
    return _Solid;
}

void PrimitiveRenderableRectangle::SetSolid(bool solid)
{
    _Solid = solid;
}

glm::vec2 PrimitiveRenderableRectangle::GetSize()
{
    return _Size;
}

void PrimitiveRenderableRectangle::SetSize(glm::vec2 size)
{
    _Size = size;
    DirtyWorldMatrix();
}

PrimitiveRenderer::PrimitiveRenderer()
{
    _Instance = this;
    
    {
        _BoxIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 6);
        _BoxVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3, 4);
        
        _BoxIndexBuffer->Lock();
        unsigned short* indicies = _BoxIndexBuffer->GetData();
        indicies[0] = 0;
        indicies[1] = 1;
        indicies[2] = 2;
        indicies[3] = 3;
        indicies[4] = 2;
        indicies[5] = 0;
        _BoxIndexBuffer->Unlock();
        
        _BoxVertexBuffer->Lock();
        Vertex_P3* vertices = static_cast<Vertex_P3*>(_BoxVertexBuffer->GetData());
        vertices[0].position[0] = -0.5;
        vertices[0].position[1] = -0.5;
        vertices[0].position[2] = 0;
        vertices[1].position[0] = -0.5;
        vertices[1].position[1] = 0.5;
        vertices[1].position[2] = 0;
        vertices[2].position[0] = 0.5;
        vertices[2].position[1] = 0.5;
        vertices[2].position[2] = 0;
        vertices[3].position[0] = 0.5;
        vertices[3].position[1] = -0.5;
        vertices[3].position[2] = 0;
        _BoxVertexBuffer->Unlock();
    }
     
    {
        _EllipseIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 32);
        _EllipseVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3, 32);
        
        _EllipseIndexBuffer->Lock();
        _EllipseVertexBuffer->Lock();
        unsigned short* indices = _EllipseIndexBuffer->GetData();
        Vertex_P3* vertices = static_cast<Vertex_P3*>(_EllipseVertexBuffer->GetData());
        
        float angle = 0.f;
        float step = M_PI * 2.f / 32.f;
        for (int i=0; i<32; i++)
        {
            indices[i] = i;
            vertices[i].position[0] = cos(angle)/2.f;
            vertices[i].position[1] = sin(angle)/2.f;
            vertices[i].position[2] = 0.f;
            angle += step;
        }
        
        _EllipseIndexBuffer->Unlock();
        _EllipseVertexBuffer->Unlock();
    }
    
    {
        _LineIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 2);
        _LineVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3, 2);
        
        _LineIndexBuffer->Lock();
        unsigned short* indexBuffer = _LineIndexBuffer->GetData();
        indexBuffer[0] = 0;
        indexBuffer[1] = 1;
        _LineIndexBuffer->Unlock();
    }
    
    Renderer::Instance()->SetHandler(PrimitiveRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/shaders/pb_solid.shc");
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    _Instance = 0;
    
    Renderer::Instance()->GetShaderManager()->UnloadShader("/shaders/pb_solid.shc");
}

PrimitiveRenderer* PrimitiveRenderer::Instance()
{
    return _Instance;
}

void PrimitiveRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    ShaderTechnique* technique = renderables[0]->GetShader()->GetTechnique(renderScheme);
    
    if (!technique)
        return;
    
    ShaderPass* shaderPass = technique->GetPass(0);
    shaderPass->Bind();
    shaderPass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
            
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    for (int i=0; i < count; i++)
    {
        PrimitiveRenderable& primitive = *static_cast<PrimitiveRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", primitive._Color);
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", primitive.GetModelViewMatrix());
         
        switch (primitive.GetPrimitiveType())
        {
            case PrimitiveRenderable::kTypeEllipse:
            {
                PrimitiveRenderableEllipse& ellipse = static_cast<PrimitiveRenderableEllipse&>(primitive);
                
                GraphicsDevice::Instance()->BindIndexBuffer(_EllipseIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_EllipseVertexBuffer);
                
                if (!ellipse._Solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 32);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangleFan, 32);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;
            }
            case PrimitiveRenderable::kTypeRectangle:
            {
                PrimitiveRenderableRectangle& rectangle = static_cast<PrimitiveRenderableRectangle&>(primitive);

                GraphicsDevice::Instance()->BindIndexBuffer(_BoxIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_BoxVertexBuffer);
                
                if (!rectangle._Solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 4);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, 6);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;
            }
            case PrimitiveRenderable::kTypeLine:
            {
                PrimitiveRenderableLine& line = static_cast<PrimitiveRenderableLine&>(primitive);
                
                _LineVertexBuffer->Lock();
                
                Vertex_P3* vertices = static_cast<Vertex_P3*>(_LineVertexBuffer->GetData());
                
                vertices[0].position[0] = line._Start[0];
                vertices[0].position[1] = line._Start[1];
                vertices[0].position[2] = line._Start[2];
                vertices[1].position[0] = line._End[0];
                vertices[1].position[1] = line._End[1];
                vertices[1].position[2] = line._End[2];
                
                _LineVertexBuffer->Unlock();
                
                GraphicsDevice::Instance()->BindIndexBuffer(_LineIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_LineVertexBuffer);
                
                GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLines, 2);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;
            }
        }
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}
