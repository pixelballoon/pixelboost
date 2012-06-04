#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/renderer/common/layer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"

using namespace pb;

PrimitiveRenderer::PrimitiveRenderer()
{
    _Effect = new Effect();
    _Effect->Load("/default/effects/primitive.fx");
    
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
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    
}

void PrimitiveRenderer::Update(float time)
{
    _Items.clear();
}

void PrimitiveRenderer::Render(RenderLayer* layer)
{
    ItemList& list = _Items[layer];
    
    if (!list.size())
        return;
        
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
            
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
#else
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
#endif
    
    EffectPass* pass = _Effect->GetTechnique(TypeHash("default"))->GetPass(0);
    
    for (ItemList::iterator it = list.begin(); it != list.end(); ++it)
    {
        glm::mat4x4 viewProjectionMatrix = layer->GetCamera()->ViewMatrix * layer->GetCamera()->ProjectionMatrix;
        pass->GetShaderProgram()->SetUniform("diffuseColor", it->color);
        
        pass->Bind();
         
        switch (it->type)
        {
            case PrimitiveInstance::kTypeEllipse:
            {
                viewProjectionMatrix = glm::translate(viewProjectionMatrix, glm::vec3(it->position, 0));
                viewProjectionMatrix = glm::scale(viewProjectionMatrix, glm::vec3(it->size, 1));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[0], glm::vec3(1,0,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[1], glm::vec3(0,1,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[2], glm::vec3(0,0,1));
                
                pass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                GraphicsDevice::Instance()->BindIndexBuffer(_EllipseIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_EllipseVertexBuffer);
                
                if (!it->solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 32);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangleFan, 32);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;

                
                break;
            }
            case PrimitiveInstance::kTypeBox:
            {
                viewProjectionMatrix = glm::translate(viewProjectionMatrix, glm::vec3(it->position, 0));
                viewProjectionMatrix = glm::scale(viewProjectionMatrix, glm::vec3(it->size, 1));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[0], glm::vec3(1,0,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[1], glm::vec3(0,1,0));
                viewProjectionMatrix = glm::rotate(viewProjectionMatrix, it->rotation[2], glm::vec3(0,0,1));

                pass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                GraphicsDevice::Instance()->BindIndexBuffer(_BoxIndexBuffer);
                GraphicsDevice::Instance()->BindVertexBuffer(_BoxVertexBuffer);
                
                if (!it->solid)
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementLineLoop, 6);
                else
                    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangleFan, 6);
                
                GraphicsDevice::Instance()->BindIndexBuffer(0);
                GraphicsDevice::Instance()->BindVertexBuffer(0);
                
                break;
            }
            case PrimitiveInstance::kTypeLine:
            {
                pass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
                
                _LineVertexBuffer->Lock();
                
                Vertex_PXYZ* vertices = static_cast<Vertex_PXYZ*>(_LineVertexBuffer->GetData());
                
                vertices[0].position[0] = it->position[0];
                vertices[0].position[1] = it->position[1];
                vertices[1].position[0] = it->size[0];
                vertices[1].position[1] = it->size[1];
                
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
    
    glDisable(GL_BLEND);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
}
    
void PrimitiveRenderer::AttachEllipse(pb::RenderLayer *layer, glm::vec2 position, glm::vec2 size, glm::vec3 rotation, glm::vec4 color)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeEllipse;
    item.position = position;
    item.size = size;
    item.rotation = rotation;
    item.color = color;
    _Items[layer].push_back(item);
}
    
void PrimitiveRenderer::AttachLine(pb::RenderLayer *layer, glm::vec2 start, glm::vec2 end, glm::vec4 color)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeLine;
    item.position = start;
    item.size = end;
    item.color = color;
    _Items[layer].push_back(item);
}
    
void PrimitiveRenderer::AttachBox(pb::RenderLayer *layer, glm::vec2 position, glm::vec2 size, glm::vec3 rotation, glm::vec4 color, bool solid)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeBox;
    item.position = position;
    item.size = size;
    item.rotation = rotation;
    item.color = color;
    item.solid = solid;
    _Items[layer].push_back(item);
}

#endif
