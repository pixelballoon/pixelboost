#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/renderer/rocket/rocketRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

#include <math.h>

using namespace pb;

RocketRenderable::RocketRenderable(Rocket::Core::Context* context)
    : Renderable(0)
    , _Context(context)
{
    
}

RocketRenderable::~RocketRenderable()
{

}

Uid RocketRenderable::GetType()
{
    return RocketRenderable::GetStaticType();
}

Uid RocketRenderable::GetStaticType()
{
    return TypeHash("RocketRenderable");
}

void RocketRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void RocketRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(glm::mat4x4());
}

Shader* RocketRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/data/shaders/pb_texturedColor.shc");
}

RocketRenderer::RocketRenderer()
{
    _MaxQuads = 1000;
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatDynamic, 6*_MaxQuads);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatDynamic, kVertexFormat_P3_C4_UV, 4*_MaxQuads);
    _VertexBuffer->Lock();
    Vertex_P3_C4_UV* vertices = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<4*_MaxQuads; i++)
    {
        vertices[i].position[2] = 0;
    }
    _VertexBuffer->Unlock();
        
    Renderer::Instance()->SetHandler(RocketRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/data/shaders/pb_texturedColor.shc");
}

RocketRenderer::~RocketRenderer()
{
    Renderer::Instance()->GetShaderManager()->UnloadShader("/data/shaders/pb_texturedColor.shc");
}
    
void RocketRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    _Viewport = viewport;
    _ShaderPass = shaderPass;
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    for (int i=0; i<count; i++)
    {
        RocketRenderable* renderable = static_cast<RocketRenderable*>(renderables[i]);
        _Renderable = renderable;
        renderable->_Context->Render();
        PurgeBuffer(true);
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}

void RocketRenderer::RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices, int* indices, int numIndices, Rocket::Core::TextureHandle textureHandle, const Rocket::Core::Vector2f& translation)
{
    Texture* texture = (Texture*)textureHandle;
    
    if (texture->HasPremultipliedAlpha())
    {
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    } else {
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
    }
    
    _VertexBuffer->Lock();
    Vertex_P3_C4_UV* vertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    for (int i=0; i<numVertices; i++)
    {
        vertexData[i].position[0] = vertices[i].position.x/32.f;
        vertexData[i].position[1] = -vertices[i].position.y/32.f;
        vertexData[i].position[2] = 0.f;
        vertexData[i].color[0] = vertices[i].colour.red/255.f;
        vertexData[i].color[1] = vertices[i].colour.green/255.f;
        vertexData[i].color[2] = vertices[i].colour.blue/255.f;
        vertexData[i].color[3] = vertices[i].colour.alpha/255.f;
        vertexData[i].uv[0] = vertices[i].tex_coord.x;
        vertexData[i].uv[1] = vertices[i].tex_coord.y;
    }
    
    _VertexBuffer->Unlock(numVertices);
    
    _IndexBuffer->Lock();
    unsigned short* indexData = _IndexBuffer->GetData();
    
    for (int i=0; i<numIndices; i++)
    {
        indexData[i] = (unsigned short)indices[i];
    }
    
    _IndexBuffer->Unlock(numIndices);
    
    glm::mat4x4 matrix = _Renderable->GetModelViewMatrix();
    matrix = glm::scale(matrix, glm::vec3(1.f,1.f,1.f)/32.f);
    matrix = glm::translate(matrix, glm::vec3(-pb::GraphicsDevice::Instance()->GetDisplayResolution().x/2.f, pb::GraphicsDevice::Instance()->GetDisplayResolution().y/2.f, 0.f));
    matrix = glm::translate(matrix, glm::vec3(translation.x, -translation.y, 0));
    _ShaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", matrix);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
    GraphicsDevice::Instance()->BindTexture(texture);
    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, _IndexBuffer->GetCurrentSize());
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
}

Rocket::Core::CompiledGeometryHandle RocketRenderer::CompileGeometry(Rocket::Core::Vertex* vertices, int numVertices, int* indices, int numIndices, Rocket::Core::TextureHandle texture)
{
    VertexBuffer* vertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P3_C4_UV, numIndices);
    
    vertexBuffer->Lock();
    Vertex_P3_C4_UV* vertexData = static_cast<Vertex_P3_C4_UV*>(vertexBuffer->GetData());
    
    for (int i=0; i<numVertices; i++)
    {
        vertexData[i].position[0] = vertices[i].position.x;
        vertexData[i].position[1] = -vertices[i].position.y;
        vertexData[i].position[2] = 0.f;
        vertexData[i].color[0] = vertices[i].colour.red/255.f;
        vertexData[i].color[1] = vertices[i].colour.green/255.f;
        vertexData[i].color[2] = vertices[i].colour.blue/255.f;
        vertexData[i].color[3] = vertices[i].colour.alpha/255.f;
        vertexData[i].uv[0] = vertices[i].tex_coord.x;
        vertexData[i].uv[1] = vertices[i].tex_coord.y;
    }
    
    vertexBuffer->Unlock(numVertices);
    
    IndexBuffer* indexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, numIndices);
    
    indexBuffer->Lock();
    unsigned short* indexData = indexBuffer->GetData();
    
    for (int i=0; i<numIndices; i++)
    {
        indexData[i] = (unsigned short)indices[i];
    }
    
    indexBuffer->Unlock(numIndices);
    
    _BufferMap[vertexBuffer] = indexBuffer;
    _TextureMap[vertexBuffer] = (Texture*)texture;
    
    return (Rocket::Core::CompiledGeometryHandle)vertexBuffer;
}

void RocketRenderer::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
    VertexBuffer* vertexBuffer = (VertexBuffer*)geometry;
    IndexBuffer* indexBuffer = _BufferMap[vertexBuffer];
    Texture* texture = _TextureMap[vertexBuffer];
    
    if (texture->HasPremultipliedAlpha())
    {
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    } else {
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
    }
    
    glm::mat4x4 matrix = _Renderable->GetModelViewMatrix();
    matrix = glm::scale(matrix, glm::vec3(1.f,1.f,1.f)/32.f);
    matrix = glm::translate(matrix, glm::vec3(-pb::GraphicsDevice::Instance()->GetDisplayResolution().x/2.f, pb::GraphicsDevice::Instance()->GetDisplayResolution().y/2.f, 0.f));
    matrix = glm::translate(matrix, glm::vec3(translation.x, -translation.y, 0));
    _ShaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", matrix);
    
    GraphicsDevice::Instance()->BindIndexBuffer(indexBuffer);
    GraphicsDevice::Instance()->BindVertexBuffer(vertexBuffer);
    GraphicsDevice::Instance()->BindTexture(texture);
    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, indexBuffer->GetCurrentSize());
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
}

void RocketRenderer::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
    VertexBuffer* vertexBuffer = (VertexBuffer*)geometry;
    IndexBuffer* indexBuffer = _BufferMap[vertexBuffer];
    _BufferMap.erase(vertexBuffer);
    _TextureMap.erase(vertexBuffer);
    
    GraphicsDevice::Instance()->DestroyIndexBuffer(indexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(vertexBuffer);
}

void RocketRenderer::EnableScissorRegion(bool enable)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, enable);
}

void RocketRenderer::SetScissorRegion(int x, int y, int width, int height)
{
    GraphicsDevice::Instance()->SetScissor(glm::vec4(x, _Viewport->GetResolution().y-y-height, width, height));
}

bool RocketRenderer::LoadTexture(Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source)
{
    std::string fileName = source.CString();
    
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    
    if (!texture->LoadFromFile(pb::kFileLocationUser, fileName, false, false))
    {
        GraphicsDevice::Instance()->DestroyTexture(texture);
        return false;
    }
    
    textureHandle = (Rocket::Core::TextureHandle)texture;
    
    textureDimensions.x = texture->GetSize().x;
    textureDimensions.y = texture->GetSize().y;
    
    return true;
}

bool RocketRenderer::GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& sourceDimensions)
{
    Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    
    if (!texture->LoadFromBytes(source, sourceDimensions.x, sourceDimensions.y, false, Texture::kTextureFormatRGBA, false))
    {
        GraphicsDevice::Instance()->DestroyTexture(texture);
        return false;
    }
    
    textureHandle = (Rocket::Core::TextureHandle)texture;
    
    return true;
}

void RocketRenderer::ReleaseTexture(Rocket::Core::TextureHandle texture)
{
    if (texture)
    {
        GraphicsDevice::Instance()->DestroyTexture((Texture*)texture);
    }
}

float RocketRenderer::GetHorizontalTexelOffset()
{
    return 0.f;
}

float RocketRenderer::GetVerticalTexelOffset()
{
    return 0.f;
}

void RocketRenderer::Release()
{
    
}

void RocketRenderer::PurgeBuffer(bool force)
{
    
}

#endif
