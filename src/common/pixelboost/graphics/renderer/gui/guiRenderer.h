#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    class IndexBuffer;
    class VertexBuffer;
    struct Vertex_P3_C4_UV;
    
    class GuiRenderable : public pb::Renderable
    {
    public:
        GuiRenderable(Uid entityUid);
        virtual ~GuiRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
        void SetTransform(const glm::mat4x4& transform);
        
    public:
        void ClearCommands();
        
        void SetScissor(bool enabled, glm::vec4 region = glm::vec4(0,0,1,1));
        void RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
        void RenderBoxFilled(glm::vec2 position, glm::vec2 size, glm::vec4 color);
        void RenderBoxOutline(glm::vec2 position, glm::vec2 size, glm::vec4 color);
        void RenderText(glm::vec2 position, const std::string& font, const std::string& string, float size, glm::vec4 color);
        
        glm::vec2 MeasureText(const std::string& font, const std::string& string, float size);
        
    private:
        struct GuiCommand
        {
            enum CommandType
            {
                kCommandTypeScissor,
                kCommandTypeLine,
                kCommandTypeBox,
                kCommandTypeText,
            } Type;
            
            GuiCommand(CommandType type)
                : Type(type)
            {
                
            }
        };

        struct GuiCommandScissor : public GuiCommand
        {
            GuiCommandScissor() : GuiCommand(kCommandTypeScissor) {}
            
            bool Enabled;
            float Region[4];
        };
        
        struct GuiCommandLine : public GuiCommand
        {
            GuiCommandLine() : GuiCommand(kCommandTypeLine) {}
            
            float Start[2];
            float End[2];
            float Color[4];
        };
        
        struct GuiCommandBox : public GuiCommand
        {
            GuiCommandBox() : GuiCommand(kCommandTypeBox) {}
            
            float Position[2];
            float Size[2];
            float Color[4];
            bool Outline;
        };
        
        struct GuiCommandText : public GuiCommand
        {
            GuiCommandText() : GuiCommand(kCommandTypeText) {}
            
            float Position[2];
            std::string Font;
            std::string String;
            float Size;
            float Color[4];
        };
        
        std::vector<GuiCommand*> _Commands;
        
        glm::mat4x4 _Transform;

    private:
        friend class GuiRenderer;
    };
    
    class GuiRenderer : public pb::IRenderer
    {
    public:
        GuiRenderer();
        virtual ~GuiRenderer();
        
        static GuiRenderer* Instance();
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
        
    private:
        void PurgeBuffer();

        IndexBuffer* _LineIndexBuffer;
        IndexBuffer* _TriangleIndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        Vertex_P3_C4_UV* _VertexData;
        
        int _ElementCount;
        int _VertexCount;
        int _MaxElements;
        int _MaxVertices;
        
        GraphicsDevice::ElementType _ElementType;

        static GuiRenderer* _Instance;
    };
}
