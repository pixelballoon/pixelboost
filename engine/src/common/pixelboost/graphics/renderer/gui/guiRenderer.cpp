#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gui/guiRenderer.h"
#include "pixelboost/graphics/renderer/text/textRenderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/maths/matrixHelpers.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

GuiRenderer* GuiRenderer::_Instance = 0;

GuiRenderable::GuiRenderable()
{
    _GeometryShader = 0;
    _SpriteShader = 0;
    _TextShader = 0;
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
    return 0;
}

void GuiRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

void GuiRenderable::SetGeometryShader(Shader* shader)
{
    _GeometryShader = shader;
}

void GuiRenderable::SetSpriteShader(Shader* shader)
{
    _SpriteShader = shader;
}

void GuiRenderable::SetTextShader(Shader* shader)
{
    _TextShader = shader;
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
    return TextRenderer::Instance()->MeasureString(font, string, size);
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
}

GuiRenderer::~GuiRenderer()
{
    GraphicsDevice::Instance()->DestroyIndexBuffer(_LineIndexBuffer);
    GraphicsDevice::Instance()->DestroyIndexBuffer(_TriangleIndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);

    _Instance = 0;
}

GuiRenderer* GuiRenderer::Instance()
{
    return _Instance;
}

void GuiRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    _BatchType = kBatchTypeUninitialised;
    _VertexCount = 0;
    _ElementCount = 0;
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    for (int i=0; i<count; i++)
    {
        GuiRenderable* renderable = static_cast<GuiRenderable*>(renderables[i]);
        
        if (!renderable->_GeometryShader || !renderable->_SpriteShader || !renderable->_TextShader)
            continue;
        
        for (const auto& command : renderable->_Commands)
        {
            // TODO : Purge buffer at a safe location
            if (_VertexCount > 1500)
            {
                PurgeBuffer(renderable, projectionMatrix, viewMatrix);
            }
            
            switch (command->Type)
            {
                case GuiRenderable::GuiCommand::kCommandTypeScissor:
                {
                    PurgeBuffer(renderable, projectionMatrix, viewMatrix);
                    
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
                        if (_BatchType != kBatchTypeGeometryLines)
                        {
                            PurgeBuffer(renderable, projectionMatrix, viewMatrix);
                            _BatchType = kBatchTypeGeometryLines;
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
                        if (_BatchType != kBatchTypeGeometryQuads)
                        {
                            PurgeBuffer(renderable, projectionMatrix, viewMatrix);
                            _BatchType = kBatchTypeGeometryQuads;
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
                    if (_BatchType != kBatchTypeGeometryLines)
                    {
                        PurgeBuffer(renderable, projectionMatrix, viewMatrix);
                        _BatchType = kBatchTypeGeometryLines;
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
                    if (_BatchType != kBatchTypeText)
                    {
                        PurgeBuffer(renderable, projectionMatrix, viewMatrix);
                        _BatchType = kBatchTypeText;
                    }
                                        
                    GuiRenderable::GuiCommandText* text = static_cast<GuiRenderable::GuiCommandText*>(command);
                    
                    Font* font = TextRenderer::Instance()->GetFont(text->Font);
                    
                    if (font)
                    {
                        _FontTexture = font->texture;
                        
                        int numElements = font->FillVertices(&_VertexData[_VertexCount], pb::kFontAlignLeft, text->String, _MaxVertices-_VertexCount,
                             glm::vec4(text->Color[0], text->Color[1], text->Color[2], text->Color[3]),
                             pb::CreateTransformMatrix(pb::kRotationOrder_XYZ, glm::vec3(text->Position[0], -text->Position[1]-text->Size, 0.f), glm::vec3(0,0,0), glm::vec3(text->Size,text->Size,1.f)));
                        _VertexCount += numElements;
                        _ElementCount += numElements/4;
                    }
                    
                    break;
                }
            }
        }
        
        PurgeBuffer(renderable, projectionMatrix, viewMatrix);
    }
    
    _VertexBuffer->Unlock(0);
    _VertexData = 0;
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}

void GuiRenderer::PurgeBuffer(GuiRenderable* renderable, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    _VertexBuffer->Unlock(_VertexCount);
    
    if (_VertexCount)
    {
        GraphicsDevice::ElementType elementType;
        
        ShaderPass* pass = 0;
        
        switch (_BatchType)
        {
            case kBatchTypeUninitialised:
                return;
            case kBatchTypeGeometryLines:
            {
                pass = renderable->_GeometryShader->GetTechnique(pb::TypeHash("default"))->GetPass(0);
                elementType = GraphicsDevice::kElementLines;
                break;
            }
            case kBatchTypeGeometryQuads:
            {
                pass = renderable->_GeometryShader->GetTechnique(pb::TypeHash("default"))->GetPass(0);
                elementType = GraphicsDevice::kElementTriangles;
                break;
            }
            case kBatchTypeSprites:
            {
                pass = renderable->_SpriteShader->GetTechnique(pb::TypeHash("default"))->GetPass(0);
                elementType = GraphicsDevice::kElementTriangles;
                break;
            }
            case kBatchTypeText:
            {
                GraphicsDevice::Instance()->BindTexture(0, _FontTexture);
                pass = renderable->_TextShader->GetTechnique(pb::TypeHash("default"))->GetPass(0);
                elementType = GraphicsDevice::kElementTriangles;
                break;
            }
        }
        
        pass->Bind();
        pass->SetEngineUniforms(projectionMatrix, viewMatrix, Engine::Instance()->GetTotalTime(), Engine::Instance()->GetGameTime());
        pass->GetShaderProgram()->SetUniform("_DiffuseColor", glm::vec4(1,0,1,1));
        pass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
        pass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable->GetModelViewMatrix());
        
        GraphicsDevice::Instance()->BindIndexBuffer(elementType == pb::GraphicsDevice::kElementTriangles ? _TriangleIndexBuffer : _LineIndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->DrawElements(elementType, elementType == pb::GraphicsDevice::kElementTriangles ? _ElementCount * 6 : _ElementCount * 2);
        
        GraphicsDevice::Instance()->BindTexture(0, 0);
        GraphicsDevice::Instance()->BindIndexBuffer(0);
        GraphicsDevice::Instance()->BindVertexBuffer(0);
    }
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    _ElementCount = 0;
    _VertexCount = 0;
}
