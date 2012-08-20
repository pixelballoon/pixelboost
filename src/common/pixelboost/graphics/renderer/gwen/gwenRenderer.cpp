#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifndef PIXELBOOST_DISABLE_GWEN

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/gwen/gwenRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"

#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include <math.h>

using namespace pb;

GwenRenderable::GwenRenderable(Gwen::Controls::Canvas* canvas)
    : pb::Renderable(0)
    , _Canvas(canvas)
{
    
}

GwenRenderable::~GwenRenderable()
{

}

Uid GwenRenderable::GetType()
{
    return GwenRenderable::GetStaticType();
}

Uid GwenRenderable::GetStaticType()
{
    return TypeHash("pb::GwenRenderable");
}

void GwenRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void GwenRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(glm::mat4x4());
}

Shader* GwenRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/default/effects/textured.fx");
}

GwenRenderer::GwenRenderer()
{
    _MaxQuads = 100;
    
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 6*_MaxQuads);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P_XYZ_UV, 4*_MaxQuads);
    _VertexBuffer->Lock();
    Vertex_PXYZ_UV* vertices = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<4*_MaxQuads; i++)
    {
        vertices[i].position[2] = 0;
    }
    _VertexBuffer->Unlock();
    
    _IndexBuffer->Lock();
    unsigned short* indicies = _IndexBuffer->GetData();
    for (int i=0; i<_MaxQuads; i++)
    {
        int offset = i * 4;
        indicies[0] = offset + 0;
        indicies[1] = offset + 1;
        indicies[2] = offset + 2;
        indicies[3] = offset + 0;
        indicies[4] = offset + 2;
        indicies[5] = offset + 3;
        indicies += 6;
    }
    _IndexBuffer->Unlock();
    
    _FontIndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 6*_MaxQuads);
    _FontVertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P_XYZ_UV, 4*_MaxQuads);
    
    _FontIndexBuffer->Lock();
    indicies = _FontIndexBuffer->GetData();
    for (int i=0; i<_MaxQuads; i++)
    {
        int offset = i * 4;
        indicies[0] = offset + 2;
        indicies[1] = offset + 1;
        indicies[2] = offset + 0;
        indicies[3] = offset + 3;
        indicies[4] = offset + 2;
        indicies[5] = offset + 0;
        indicies += 6;
    }
    _FontIndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(GwenRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/default/effects/textured.fx");
}

GwenRenderer::~GwenRenderer()
{
    Renderer::Instance()->GetShaderManager()->UnloadShader("/default/effects/textured.fx");
}
    
void GwenRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    _Viewport = viewport;
    _ShaderPass = shaderPass;
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseColor", glm::vec4(1,1,1,1));
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseTexture", 0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
#ifndef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
#else
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
#endif
    
    for (int i=0; i<count; i++)
    {
        GwenRenderable* renderable = static_cast<GwenRenderable*>(renderables[i]);
        _Renderable = renderable;
        renderable->_Canvas->RenderCanvas();
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}

void GwenRenderer::Init()
{
    
}

void GwenRenderer::Begin()
{
    _VertexCount = 0;
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
}

void GwenRenderer::End()
{
    PurgeBuffer(true);
    _VertexBuffer->Unlock(0);
}

void GwenRenderer::SetDrawColor(Gwen::Color colour)
{
    _Color = glm::vec4(colour.r, colour.g, colour.b, colour.a)/255.f;
}

void GwenRenderer::DrawFilledRect( Gwen::Rect rect )
{
    Translate(rect);
    
    PurgeBuffer(true);
    
    _VertexData[0].position[0] = (rect.x)/32.f;
    _VertexData[0].position[1] = (-rect.y)/32.f;
    _VertexData[0].uv[0] = 0;
    _VertexData[0].uv[1] = 1;
    _VertexData[1].position[0] = (rect.x)/32.f;
    _VertexData[1].position[1] = (-rect.y - rect.h)/32.f;
    _VertexData[1].uv[0] = 0;
    _VertexData[1].uv[1] = 1;
    _VertexData[2].position[0] = (rect.x + rect.w)/32.f;
    _VertexData[2].position[1] = (-rect.y - rect.h)/32.f;
    _VertexData[2].uv[0] = 0;
    _VertexData[2].uv[1] = 1;
    _VertexData[3].position[0] = (rect.x + rect.w)/32.f;
    _VertexData[3].position[1] = (-rect.y)/32.f;
    _VertexData[3].uv[0] = 0;
    _VertexData[3].uv[1] = 1;
    
    _VertexData += 4;
    _VertexCount += 4;
    
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseColor", _Color);

    PurgeBuffer(true);
    
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseColor", glm::vec4(1,1,1,1));
}

void GwenRenderer::StartClip()
{
    PurgeBuffer(true);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, true);
    Gwen::Rect clip = ClipRegion();
    GraphicsDevice::Instance()->SetScissor(glm::vec4(clip.x, _Viewport->GetResolution().y-clip.y-clip.h, clip.w, clip.h));
}

void GwenRenderer::EndClip()
{
    PurgeBuffer(true);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, false);
}

void GwenRenderer::DrawTexturedRect(Gwen::Texture* definition, Gwen::Rect rect, float u1, float v1, float u2, float v2)
{
    Translate(rect);
    
    pb::Texture* texture = static_cast<pb::Texture*>(definition->data);
    
    if (GraphicsDevice::Instance()->GetBoundTexture() != texture)
        PurgeBuffer(true);
    
    GraphicsDevice::Instance()->BindTexture(texture);
    
    _VertexData[0].position[0] = (rect.x)/32.f;
    _VertexData[0].position[1] = (-rect.y)/32.f;
    _VertexData[0].uv[0] = u1;
    _VertexData[0].uv[1] = v1;
    _VertexData[1].position[0] = (rect.x)/32.f;
    _VertexData[1].position[1] = (-rect.y - rect.h)/32.f;
    _VertexData[1].uv[0] = u1;
    _VertexData[1].uv[1] = v2;
    _VertexData[2].position[0] = (rect.x + rect.w)/32.f;
    _VertexData[2].position[1] = (-rect.y - rect.h)/32.f;
    _VertexData[2].uv[0] = u2;
    _VertexData[2].uv[1] = v2;
    _VertexData[3].position[0] = (rect.x + rect.w)/32.f;
    _VertexData[3].position[1] = (-rect.y)/32.f;
    _VertexData[3].uv[0] = u2;
    _VertexData[3].uv[1] = v1;
    
    _VertexData += 4;
    _VertexCount += 4;
}

void GwenRenderer::LoadTexture(Gwen::Texture* definition)
{
    std::string fileName = definition->name.c_str();

    pb::Texture* texture = GraphicsDevice::Instance()->CreateTexture();
    
    if (fileName[0] != '/')
    {
        fileName = "/data/" + fileName;
        texture->LoadFromPng(pb::kFileLocationBundle, fileName, false);
    } else {
        texture->LoadFromPng(pb::kFileLocationUser, fileName, false);
    }
    
    definition->width = texture->GetSize()[0];
    definition->height = texture->GetSize()[1];
    definition->data = texture;
}

void GwenRenderer::FreeTexture(Gwen::Texture* definition)
{
    GraphicsDevice::Instance()->DestroyTexture((pb::Texture*)definition->data);
}

Gwen::Color GwenRenderer::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default )
{
    #ifdef PIXELBOOST_GRAPHICS_OPENGL2
        // TODO: Properly implement this - it's currently taken from the OpenGL sample code, and isn't particularly nice

        pb::Texture* texture = static_cast<pb::Texture*>(pTexture->data);

        pb::Texture* prevTexture = GraphicsDevice::Instance()->BindTexture(texture);

        if (!pTexture->width)
            return col_default;

        unsigned int iPixelSize = sizeof(unsigned char) * 4;

        unsigned char* data = (unsigned char*) malloc( iPixelSize * pTexture->width * pTexture->height );

        glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        unsigned int iOffset = (y * pTexture->width + x) * 4;

        Gwen::Color c;
        c.r = data[0 + iOffset];
        c.g = data[1 + iOffset];
        c.b = data[2 + iOffset];
        c.a = data[3 + iOffset];

        free( data );
    
        GraphicsDevice::Instance()->BindTexture(prevTexture);

        return c;
    #else
        return Gwen::Color();
    #endif
}

void GwenRenderer::RenderText(Gwen::Font* font, Gwen::Point pos, const Gwen::UnicodeString& text)
{
    PurgeBuffer(true);
    
    Translate(pos.x, pos.y);

    Gwen::String fontName = Gwen::Utility::UnicodeToString(font->facename);

    pb::Font* renderFont = pb::Game::Instance()->GetFontRenderer()->LoadFont(pb::kFileLocationBundle, fontName, "/data/fonts/"+fontName, false);

    float size = font->size * Scale();
    if (!text.length())
        return;
    
    renderFont->FillVertexBuffer(_FontVertexBuffer, Gwen::Utility::UnicodeToString(text));
    
    glm::mat4x4 modelMatrix = glm::translate(glm::mat4x4(), glm::vec3(pos.x, -pos.y - size, 0)/32.f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, size, 1)/32.f);
    
    _ShaderPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", _Renderable->GetMVP() * modelMatrix);
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseColor", glm::vec4(0,0,0,1));
    
    pb::Texture* prevTexture = GraphicsDevice::Instance()->BindTexture(renderFont->texture);
    GraphicsDevice::Instance()->BindIndexBuffer(_FontIndexBuffer);
    GraphicsDevice::Instance()->BindVertexBuffer(_FontVertexBuffer);
    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (_FontVertexBuffer->GetCurrentSize()/4)*6);
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(prevTexture);
    
    _ShaderPass->GetShaderProgram()->SetUniform("diffuseColor", glm::vec4(1,1,1,1));
}

Gwen::Point GwenRenderer::MeasureText(Gwen::Font* font, const Gwen::UnicodeString& text)
{
    Gwen::String fontName = Gwen::Utility::UnicodeToString(font->facename);
    
    pb::Game::Instance()->GetFontRenderer()->LoadFont(pb::kFileLocationBundle, fontName, "/data/fonts/"+fontName, false);
    
    float size = font->size * Scale();
    float length = pb::Game::Instance()->GetFontRenderer()->MeasureString(fontName, Gwen::Utility::UnicodeToString(text), size/32.f).x*32.f;
    
    return Gwen::Point(length, size);
}

void GwenRenderer::PurgeBuffer(bool force)
{
    if (!force && _VertexCount < _MaxQuads*4)
        return;
    
    _VertexBuffer->Unlock(_VertexCount);
    if (_VertexCount)
    {
        _ShaderPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", _Renderable->GetMVP());
        
        GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (_VertexCount/4)*6);
        _VertexCount = 0;
    }
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
}

#endif

#endif
