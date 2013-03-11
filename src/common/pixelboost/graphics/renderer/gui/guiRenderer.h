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
        void SetScissor(bool Enabled, glm::vec4 region);
        void RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
        void RenderBox(glm::vec2 topLeft, glm::vec2 bottomRight, glm::vec4 color);
        void RenderText(glm::vec2 position, const char* text, glm::vec4 color);
        
    private:
        struct GuiCommandScissor
        {
            bool Enabled;
            float Region[4];
        };
        
        struct GuiCommandLine
        {
            float Start[2];
            float End[2];
            float Color[4];
        };
        
        struct GuiCommandBox
        {
            float TopLeft[2];
            float BottomRight[2];
            float Color[4];
        };
        
        struct GuiCommandText
        {
            float Position[2];
            char* Text;
            float Color[4];
        };
        
        struct GuiCommand
        {
            enum CommandType
            {
                kCommandTypeScissor,
                kCommandTypeLine,
                kCommandTypeBox,
                kCommandTypeText,
            } Type;
            
            union
            {
                GuiCommandScissor Scissor;
                GuiCommandLine Line;
                GuiCommandBox Box;
                GuiCommandText Text;
            };
        };
        
        std::vector<GuiCommand> _Commands;
        
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
        
        GraphicsDevice::ElementType _ElementType;
        int _ElementCount;

        int _MaxElements;
        int _MaxQuads;
        
        static GuiRenderer* _Instance;
    };
}
