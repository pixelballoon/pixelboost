#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

GuiRenderer* GuiRenderer::_Instance = 0;

GuiRenderable::GuiRenderable(Uid entityUid)
    : pb::Renderable(entityUid)
{
    
}

GuiRenderable::~GuiRenderable()
{
    
}

Uid GuiRenderable::GetType()
{
    return GuiRenderable::GetStaticType();
}

Uid GuiRenderable::GetStaticType()
{
    return TypeHash("pb::GuiRenderable");
}

void GuiRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void GuiRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(_Transform * CreateScaleMatrix(glm::vec3(1.f/32.f, 1.f/32.f, 1.f/32.f)));
}

Shader* GuiRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/shaders/pb_solidColor.shc");
}

void GuiRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

void GuiRenderable::ClearCommands()
{
    for (const auto& command : _Commands)
    {
        delete command;
    }
    _Commands.clear();
}

void GuiRenderable::SetScissor(bool enabled, glm::vec4 region)
{
    GuiCommandScissor* scissor = new GuiCommandScissor();
    scissor->Enabled = enabled;
    scissor->Region[0] = region.x;
    scissor->Region[1] = region.y;
    scissor->Region[2] = region.z;
    scissor->Region[3] = region.w;
    _Commands.push_back(scissor);
}

void GuiRenderable::RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color)
{
    GuiCommandLine* line = new GuiCommandLine();
    line->Start[0] = start.x;
    line->Start[1] = start.y;
    line->End[0] = end.x;
    line->End[1] = end.y;
    line->Color[0] = color.r;
    line->Color[1] = color.g;
    line->Color[2] = color.b;
    line->Color[3] = color.a;
    _Commands.push_back(line);
}

void GuiRenderable::RenderBoxFilled(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    GuiCommandBox* box = new GuiCommandBox(); 
    box->Position[0] = position.x;
    box->Position[1] = position.y;
    box->Size[0] = size.x;
    box->Size[1] = size.y;
    box->Color[0] = color.r;
    box->Color[1] = color.g;
    box->Color[2] = color.b;
    box->Color[3] = color.a;
    box->Outline = false;
    _Commands.push_back(box);
}

void GuiRenderable::RenderBoxOutline(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    GuiCommandBox* box = new GuiCommandBox();
    box->Position[0] = position.x;
    box->Position[1] = position.y;
    box->Size[0] = size.x;
    box->Size[1] = size.y;
    box->Color[0] = color.r;
    box->Color[1] = color.g;
    box->Color[2] = color.b;
    box->Color[3] = color.a;
    box->Outline = true;
    _Commands.push_back(box);
}

void GuiRenderable::RenderText(glm::vec2 position, const std::string& font, const std::string& string, float size, glm::vec4 color)
{
    GuiCommandText* text = new GuiCommandText();
    text->Position[0] = position.x;
    text->Position[1] = position.y;
    text->Color[0] = color.r;
    text->Color[1] = color.g;
    text->Color[2] = color.b;
    text->Color[3] = color.a;
    text->Font = font;
    text->String = string;
    text->Size = size;
    _Commands.push_back(text);
}

glm::vec2 GuiRenderable::MeasureText(const std::string& font, const std::string& string, float size)
{
    return FontRenderer::Instance()->MeasureString(font, string, size);
}

GuiRenderer::GuiRenderer()
{
    PbAssert(!_Instance);
    
    _Instance = this;
    
    _MaxElements = 500;
    _MaxVertices = 4*_MaxElements;
    
    _LineIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 2*_MaxElements);
    _TriangleIndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, 6*_MaxElements);
    
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatDynamic, kVertexFormat_P3_C4_UV, 4*_MaxElements);
    _VertexBuffer->Lock();
    Vertex_P3_C4_UV* vertices = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<4*_MaxElements; i++)
    {
        vertices[i].position[2] = 0;
    }
    _VertexBuffer->Unlock();
    
    unsigned short* indicies = 0;
    _LineIndexBuffer->Lock();
    indicies = _LineIndexBuffer->GetData();
    for (int i=0; i<_MaxElements*2; i++)
    {
        indicies[i] = i;
    }
    _LineIndexBuffer->Unlock();
    
    _TriangleIndexBuffer->Lock();
    indicies = _TriangleIndexBuffer->GetData();
    for (int i=0; i<_MaxElements; i++)
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
    _TriangleIndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(GuiRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/shaders/pb_solidColor.shc");
}

GuiRenderer::~GuiRenderer()
{
    _Instance = 0;
    
    Renderer::Instance()->GetShaderManager()->UnloadShader("/shaders/pb_solidColor.shc");
}

GuiRenderer* GuiRenderer::Instance()
{
    return _Instance;
}

void GuiRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    ShaderTechnique* technique = renderables[0]->GetShader()->GetTechnique(renderScheme);
    
    if (!technique)
        return;
    
    ShaderPass* shaderPass = technique->GetPass(0);
    shaderPass->Bind();
    shaderPass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", glm::vec4(1,1,1,1));
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    _ElementType = pb::GraphicsDevice::kElementTriangles;
    _VertexCount = 0;
    _ElementCount = 0;
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    for (int i=0; i<count; i++)
    {
        GuiRenderable* renderable = static_cast<GuiRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable->GetModelViewMatrix());

        for (const auto& command : renderable->_Commands)
        {
            // TODO : Purge buffer at a safe location
            if (_VertexCount > 1500)
            {
                PurgeBuffer();
            }
            
            switch (command->Type)
            {
                case GuiRenderable::GuiCommand::kCommandTypeScissor:
                {
                    PurgeBuffer();
                    
                    GuiRenderable::GuiCommandScissor* scissor = static_cast<GuiRenderable::GuiCommandScissor*>(command);
                    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, scissor->Enabled);
                    GraphicsDevice::Instance()->SetScissor(glm::vec4(scissor->Region[0], scissor->Region[1], scissor->Region[2], scissor->Region[3]));
                    break;
                }
                case GuiRenderable::GuiCommand::kCommandTypeBox:
                {
                    GuiRenderable::GuiCommandBox* box = static_cast<GuiRenderable::GuiCommandBox*>(command);
                    
                    if (box->Outline)
                    {
                        if (_ElementType != GraphicsDevice::kElementLines)
                        {
                            PurgeBuffer();
                            _ElementType = GraphicsDevice::kElementLines;
                        }
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _ElementCount += 4;
                    } else {
                        if (_ElementType != GraphicsDevice::kElementTriangles)
                        {
                            PurgeBuffer();
                            _ElementType = GraphicsDevice::kElementTriangles;
                        }
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _VertexData[_VertexCount].position[0] = box->Position[0] + box->Size[0];
                        _VertexData[_VertexCount].position[1] = -box->Position[1] - box->Size[1];
                        _VertexData[_VertexCount].color[0] = box->Color[0];
                        _VertexData[_VertexCount].color[1] = box->Color[1];
                        _VertexData[_VertexCount].color[2] = box->Color[2];
                        _VertexData[_VertexCount].color[3] = box->Color[3];
                        _VertexCount++;
                        
                        _ElementCount++;
                    }
                    
                    break;
                }
                case GuiRenderable::GuiCommand::kCommandTypeLine:
                {
                    if (_ElementType != GraphicsDevice::kElementLines)
                    {
                        PurgeBuffer();
                        _ElementType = GraphicsDevice::kElementLines;
                    }
                    
                    GuiRenderable::GuiCommandLine* line = static_cast<GuiRenderable::GuiCommandLine*>(command);
                    
                    _VertexData[_VertexCount].position[0] = line->Start[0];
                    _VertexData[_VertexCount].position[1] = -line->Start[1];
                    _VertexData[_VertexCount].color[0] = line->Color[0];
                    _VertexData[_VertexCount].color[1] = line->Color[1];
                    _VertexData[_VertexCount].color[2] = line->Color[2];
                    _VertexData[_VertexCount].color[3] = line->Color[3];
                    _VertexCount++;
                    
                    _VertexData[_VertexCount].position[0] = line->End[0];
                    _VertexData[_VertexCount].position[1] = -line->End[1];
                    _VertexData[_VertexCount].color[0] = line->Color[0];
                    _VertexData[_VertexCount].color[1] = line->Color[1];
                    _VertexData[_VertexCount].color[2] = line->Color[2];
                    _VertexData[_VertexCount].color[3] = line->Color[3];
                    _VertexCount++;
                    
                    _ElementCount++;
                    
                    break;
                }
                case GuiRenderable::GuiCommand::kCommandTypeText:
                {
                    //if (_ElementType != GraphicsDevice::kElementTriangles)
                    {
                        PurgeBuffer();
                        _ElementType = GraphicsDevice::kElementTriangles;
                    }
                                        
                    GuiRenderable::GuiCommandText* text = static_cast<GuiRenderable::GuiCommandText*>(command);
                    
                    Font* font = FontRenderer::Instance()->GetFont(text->Font);
                    
                    if (font)
                    {
                        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
                        GraphicsDevice::Instance()->BindTexture(font->texture);
                        
                        int numElements = font->FillVertices(&_VertexData[_VertexCount], text->String, _MaxVertices-_VertexCount,
                             glm::vec4(text->Color[0], text->Color[1], text->Color[2], text->Color[3]),
                             pb::CreateTransformMatrix(pb::kRotationOrder_XYZ, glm::vec3(text->Position[0], -text->Position[1]-text->Size/2.f, 0.f), glm::vec3(0,0,0), glm::vec3(text->Size,text->Size,1.f)));
                        _VertexCount += numElements;
                        _ElementCount += numElements/4;
                        
                        PurgeBuffer();
                        
                        GraphicsDevice::Instance()->BindTexture(0);
                        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
                    }
                    
                    break;
                }
            }
        }
    }
    
    PurgeBuffer();
    
    _VertexBuffer->Unlock(0);
    _VertexData = 0;
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}

void GuiRenderer::PurgeBuffer()
{
    _VertexBuffer->Unlock(_VertexCount);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_ElementType == pb::GraphicsDevice::kElementTriangles ? _TriangleIndexBuffer : _LineIndexBuffer);
    GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
    
    GraphicsDevice::Instance()->DrawElements(_ElementType, _ElementType == pb::GraphicsDevice::kElementTriangles ? _ElementCount * 6 : _ElementCount * 2);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    _ElementCount = 0;
    _VertexCount = 0;
}
