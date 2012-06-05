#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"

using namespace pb;

PrimitiveRenderable::PrimitiveRenderable(Uid entityUid)
    : Renderable(entityUid)
{
    
}


Uid PrimitiveRenderable::GetRenderableType()
{
    return TypeHash("primitive");
}

Effect* PrimitiveRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/primitive.fx");
}

PrimitiveRenderableEllipse::PrimitiveRenderableEllipse(Uid entityUid)
    : PrimitiveRenderable(entityUid)
    , Solid(false)
{
    
}

PrimitiveRenderable::Type PrimitiveRenderableEllipse::GetPrimitiveType()
{
    return kTypeEllipse;
}

PrimitiveRenderableLine::PrimitiveRenderableLine(Uid entityUid)
    : PrimitiveRenderable(entityUid)
{
    
}


PrimitiveRenderable::Type PrimitiveRenderableLine::GetPrimitiveType()
{
    return kTypeLine;
}

PrimitiveRenderableRectangle::PrimitiveRenderableRectangle(Uid entityUid)
    : PrimitiveRenderable(entityUid)
    , Solid(false)
{
    
}


PrimitiveRenderable::Type PrimitiveRenderableRectangle::GetPrimitiveType()
{
    return kTypeRectangle;
}

PrimitiveRenderer::PrimitiveRenderer()
{
    {
        _BoxIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 6);
        _BoxVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P_XYZ, 4);
        
        _BoxIndexBuffer->Lock();
        unsigned short* indicies = _BoxIndexBuffer->GetData();
        indicies[0] = 0;
        indicies[1] = 1;
        indicies[2] = 2;
        indicies[3] = 1;
        indicies[4] = 2;
        indicies[5] = 3;
        _BoxIndexBuffer->Unlock();
        
        _BoxVertexBuffer->Lock();
        Vertex_PXYZ* vertices = static_cast<Vertex_PXYZ*>(_BoxVertexBuffer->GetData());
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
        _EllipseVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P_XYZ, 32);
        
        _EllipseIndexBuffer->Lock();
        _EllipseVertexBuffer->Lock();
        unsigned short* indices = _EllipseIndexBuffer->GetData();
        Vertex_PXYZ* vertices = static_cast<Vertex_PXYZ*>(_EllipseVertexBuffer->GetData());
        
        float angle = 0.f;
        float step = M_PI * 2.f / 32.f;
        for (int i=0; i<32; i++)
        {
            indices[i] = i;
            vertices[i].position[0] = cos(angle)*0.5;
            vertices[i].position[1] = sin(angle)*0.5;
            vertices[i].position[2] = 0.f;
            angle += step;
        }
        
        _EllipseIndexBuffer->Unlock();
        _EllipseVertexBuffer->Unlock();
    }
    
    {
        _LineIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 2);
        _LineVertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P_XYZ, 2);
        
        _LineIndexBuffer->Lock();
        unsigned short* indexBuffer = _LineIndexBuffer->GetData();
        indexBuffer[0] = 0;
        indexBuffer[1] = 1;
        _LineIndexBuffer->Unlock();
    }
    
    Renderer::Instance()->SetHandler(TypeHash("primitive"), this);
    
    Renderer::Instance()->GetEffectManager()->LoadEffect("/default/effects/primitive.fx");
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    Renderer::Instance()->GetEffectManager()->UnloadEffect("/default/effects/primitive.fx");
}

void PrimitiveRenderer::Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
            
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
#else
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
#endif
    
    for (int i=0; i < count; i++)
    {
        PrimitiveRenderable& primitive = *static_cast<PrimitiveRenderable*>(renderables[i]);
        
        glm::mat4x4 viewProjectionMatrix = viewport->GetCamera()->ProjectionMatrix * viewport->GetCamera()->ViewMatrix;
        effectPass->GetShaderProgram()->SetUniform("diffuseColor", primitive.Color);
         
        switch (primitive.GetPrimitiveType())
        {
            case PrimitiveRenderable::kTypeEllipse:
            {
                PrimitiveRenderableEllipse& ellipse = static_cast<PrimitiveRenderableEllipse&>(primitive);
                viewProjectionMatrix = glm::translate(viewProjectionMatrix, ellipse.Position);
                viewProjectionMatrix = glm::scale(viewProjectionMatrix, glm::vec3(ellipse.Size, 1));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, ellipse.Rotation[0], glm::vec3(1,0,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, ellipse.Rotation[1], glm::vec3(0,1,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, ellipse.Rotation[2], glm::vec3(0,0,1));
                
                effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                GraphicsDevice::Instance()->BindIndexBuffer(_EllipseIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_EllipseVertexBuffer);
                
                if (!ellipse.Solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 32);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangleFan, 32);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;

                
                break;
            }
            case PrimitiveRenderable::kTypeRectangle:
            {
                PrimitiveRenderableRectangle& rectangle = static_cast<PrimitiveRenderableRectangle&>(primitive);
                viewProjectionMatrix = glm::translate(viewProjectionMatrix, rectangle.Position);
                viewProjectionMatrix = glm::scale(viewProjectionMatrix, glm::vec3(rectangle.Size, 1));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, rectangle.Rotation[0], glm::vec3(1,0,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, rectangle.Rotation[1], glm::vec3(0,1,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, rectangle.Rotation[2], glm::vec3(0,0,1));

                effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                GraphicsDevice::Instance()->BindIndexBuffer(_BoxIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_BoxVertexBuffer);
                
                if (!rectangle.Solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 6);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangleFan, 6);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;
            }
            case PrimitiveRenderable::kTypeLine:
            {
                PrimitiveRenderableLine& line = static_cast<PrimitiveRenderableLine&>(primitive);
                
                effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                
                _LineVertexBuffer->Lock();
                
                Vertex_PXYZ* vertices = static_cast<Vertex_PXYZ*>(_LineVertexBuffer->GetData());
                
                vertices[0].position[0] = line.Start[0];
                vertices[0].position[1] = line.Start[1];
                vertices[0].position[2] = line.Start[2];
                vertices[1].position[0] = line.End[0];
                vertices[1].position[1] = line.End[1];
                vertices[1].position[2] = line.End[2];
                
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

#endif
