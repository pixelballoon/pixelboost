#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
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
    _Commands.clear();
}

void GuiRenderable::SetScissor(bool enabled, glm::vec4 region)
{
    GuiCommand command;
    command.Type = GuiCommand::kCommandTypeScissor;
    command.Scissor.Enabled = enabled;
    command.Scissor.Region[0] = region.x;
    command.Scissor.Region[1] = region.y;
    command.Scissor.Region[2] = region.z;
    command.Scissor.Region[3] = region.w;
    _Commands.push_back(command);
}

void GuiRenderable::RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color)
{
    GuiCommand command;
    command.Type = GuiCommand::kCommandTypeLine;
    command.Line.Start[0] = start.x;
    command.Line.Start[1] = start.y;
    command.Line.End[0] = end.x;
    command.Line.End[1] = end.y;
    command.Line.Color[0] = color.r;
    command.Line.Color[1] = color.g;
    command.Line.Color[2] = color.b;
    command.Line.Color[3] = color.a;
    _Commands.push_back(command);
}

void GuiRenderable::RenderBoxFilled(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    GuiCommand command;
    command.Type = GuiCommand::kCommandTypeBox;
    command.Box.Position[0] = position.x;
    command.Box.Position[1] = position.y;
    command.Box.Size[0] = size.x;
    command.Box.Size[1] = size.y;
    command.Box.Color[0] = color.r;
    command.Box.Color[1] = color.g;
    command.Box.Color[2] = color.b;
    command.Box.Color[3] = color.a;
    command.Box.Outline = false;
    _Commands.push_back(command);
}

void GuiRenderable::RenderBoxOutline(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
    GuiCommand command;
    command.Type = GuiCommand::kCommandTypeBox;
    command.Box.Position[0] = position.x;
    command.Box.Position[1] = position.y;
    command.Box.Size[0] = size.x;
    command.Box.Size[1] = size.y;
    command.Box.Color[0] = color.r;
    command.Box.Color[1] = color.g;
    command.Box.Color[2] = color.b;
    command.Box.Color[3] = color.a;
    command.Box.Outline = true;
    _Commands.push_back(command);
}

void GuiRenderable::RenderText(glm::vec2 position, const char* text, glm::vec4 color)
{
    
}

GuiRenderer::GuiRenderer()
{
    _Instance = this;
    
    _MaxQuads = 4000;
    _MaxElements = 6*_MaxQuads;
    
    _LineIndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 2*_MaxQuads);
    _TriangleIndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 6*_MaxQuads);
    
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_C4_UV, 4*_MaxQuads);
    _VertexBuffer->Lock();
    Vertex_P3_C4_UV* vertices = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    for (int i=0; i<4*_MaxQuads; i++)
    {
        vertices[i].position[2] = 0;
    }
    _VertexBuffer->Unlock();
    
    unsigned short* indicies = 0;
    _LineIndexBuffer->Lock();
    indicies = _LineIndexBuffer->GetData();
    for (int i=0; i<_MaxQuads*2; i++)
    {
        indicies[i] = i;
    }
    _LineIndexBuffer->Unlock();
    
    _TriangleIndexBuffer->Lock();
    indicies = _TriangleIndexBuffer->GetData();
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

void GuiRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", glm::vec4(1,1,1,1));
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    
    _ElementType = pb::GraphicsDevice::kElementTriangles;
    _ElementCount = 0;
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    for (int i=0; i<count; i++)
    {
        GuiRenderable* renderable = static_cast<GuiRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable->GetModelViewMatrix());

        for (const auto& command : renderable->_Commands)
        {
            switch (command.Type)
            {
                case GuiRenderable::GuiCommand::kCommandTypeScissor:
                {
                    PurgeBuffer();
                    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateScissor, command.Scissor.Enabled);
                    GraphicsDevice::Instance()->SetScissor(glm::vec4(command.Scissor.Region[0], command.Scissor.Region[1], command.Scissor.Region[2], command.Scissor.Region[3]));
                    break;
                }
                case GuiRenderable::GuiCommand::kCommandTypeBox:
                {
                    if (command.Box.Outline)
                    {
                        if (_ElementType != GraphicsDevice::kElementLines)
                        {
                            PurgeBuffer();
                            _ElementType = GraphicsDevice::kElementLines;
                        }
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0] + command.Box.Size[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0] + command.Box.Size[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0] + command.Box.Size[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1] - command.Box.Size[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0] + command.Box.Size[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1] - command.Box.Size[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1] - command.Box.Size[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1] - command.Box.Size[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
                        _ElementCount++;
                        
                        _VertexData[_ElementCount].position[0] = command.Box.Position[0];
                        _VertexData[_ElementCount].position[1] = -command.Box.Position[1];
                        _VertexData[_ElementCount].color[0] = command.Box.Color[0];
                        _VertexData[_ElementCount].color[1] = command.Box.Color[1];
                        _VertexData[_ElementCount].color[2] = command.Box.Color[2];
                        _VertexData[_ElementCount].color[3] = command.Box.Color[3];
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
                    
                    _VertexData[_ElementCount].position[0] = command.Line.Start[0];
                    _VertexData[_ElementCount].position[1] = -command.Line.Start[1];
                    _VertexData[_ElementCount].color[0] = command.Line.Color[0];
                    _VertexData[_ElementCount].color[1] = command.Line.Color[1];
                    _VertexData[_ElementCount].color[2] = command.Line.Color[2];
                    _VertexData[_ElementCount].color[3] = command.Line.Color[3];
                    _ElementCount++;
                    
                    _VertexData[_ElementCount].position[0] = command.Line.End[0];
                    _VertexData[_ElementCount].position[1] = -command.Line.End[1];
                    _VertexData[_ElementCount].color[0] = command.Line.Color[0];
                    _VertexData[_ElementCount].color[1] = command.Line.Color[1];
                    _VertexData[_ElementCount].color[2] = command.Line.Color[2];
                    _VertexData[_ElementCount].color[3] = command.Line.Color[3];
                    _ElementCount++;
                    
                    break;
                }
                case GuiRenderable::GuiCommand::kCommandTypeText:
                {
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
    _VertexBuffer->Unlock(_ElementCount);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_ElementType == pb::GraphicsDevice::kElementTriangles ? _TriangleIndexBuffer : _LineIndexBuffer);
    GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
    
    GraphicsDevice::Instance()->DrawElements(_ElementType, _ElementCount);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    _VertexBuffer->Lock();
    _VertexData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    _ElementCount = 0;
}
