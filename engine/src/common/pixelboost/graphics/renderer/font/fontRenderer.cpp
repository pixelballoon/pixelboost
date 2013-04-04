#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/util/localisation/string.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

FontRenderer* FontRenderer::_Instance = 0;

FontRenderable::FontRenderable()
{
    Offset = 0.f;
    Alignment = kFontAlignCenter;
    Size = 1.f;
    Tint = glm::vec4(1,1,1,1);
}

FontRenderable::~FontRenderable()
{
}

Uid FontRenderable::GetType()
{
    return FontRenderable::GetStaticType();
}

Uid FontRenderable::GetStaticType()
{
    return TypeHash("pb::FontRenderable");
}

void FontRenderable::CalculateBounds()
{
    glm::vec4 position = GetWorldMatrix() * glm::vec4(0,0,0,1);
    glm::vec2 size = FontRenderer::Instance()->MeasureString(Font, Text, Size);
    BoundingSphere bounds(glm::vec3(position.x, position.y, position.z), glm::max(size.x, size.y));
    SetBounds(bounds);
}

void FontRenderable::CalculateWorldMatrix()
{
    glm::mat4x4 worldMatrix = glm::scale(glm::mat4x4(), glm::vec3(Size, Size, 1));
    worldMatrix = glm::translate(worldMatrix, glm::vec3(Offset, 0, 0));
    worldMatrix = Transform * worldMatrix;
    SetWorldMatrix(worldMatrix);
}

Shader* FontRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return ResourceManager::Instance()->GetPool("pixelboost")->GetResource<ShaderResource>("/shaders/pb_textured.shc")->GetShader();
}

void FontRenderable::SetFont(const std::string& font)
{
    Font = font;
    CalculateOffset();
}

const std::string& FontRenderable::GetFont()
{
    return Font;
}

void FontRenderable::SetText(const std::string& text)
{
    Text = text;
    CalculateOffset();
}

const std::string& FontRenderable::GetText()
{
    return Text;
}

void FontRenderable::SetTint(const glm::vec4& tint)
{
    Tint = tint;
}

const glm::vec4& FontRenderable::GetTint()
{
    return Tint;
}

void FontRenderable::SetSize(float size)
{
    Size = size;
    CalculateOffset();
}

float FontRenderable::GetSize()
{
    return Size;
}

void FontRenderable::SetTransform(const glm::mat4x4& transform)
{
    Transform = transform;
    DirtyWorldMatrix();
    DirtyBounds();
}

const glm::mat4x4& FontRenderable::GetTransform()
{
    return Transform;
}

void FontRenderable::SetAlignment(FontAlign alignment)
{
    Alignment = alignment;
    CalculateOffset();
}

FontAlign FontRenderable::GetAlignment()
{
    return Alignment;
}

void FontRenderable::CalculateOffset()
{
    Offset = FontRenderer::Instance()->MeasureString(Font, Text, 1.f).x;
    
    switch (Alignment) {
        case kFontAlignLeft:
            Offset = 0;
            break;
        case kFontAlignCenter:
            Offset = -Offset/2.f;
            break;
        case kFontAlignRight:
            Offset = -Offset;
            break;
    }
    
    DirtyWorldMatrix();
    DirtyBounds();
}

int Font::FillVertices(Vertex_P3_C4_UV* vertices, const std::string& string, int maxVertices, glm::vec4 color, const glm::mat4x4& transform, glm::vec2* measuredSize)
{
    wstring wideString = UTF8toUTF32(string);
    
    int numCharacters = 0;
    
    float maxLineHeight = 0.f;
    float maxLineLength = 0.f;
    float offsetX = 0.f;
    float offsetY = 0.f;
    for (int i=0; i<wideString.length(); i++)
    {
        if (wideString[i] == '\n')
        {
            maxLineLength = glm::max(offsetX, maxLineLength);
            offsetX = 0.f;
            offsetY -= 1.f;
        } else {
            std::map<uint32_t, Font::Character>::iterator charIt = chars.find(wideString[i]);
            
            if (charIt == chars.end())
                continue;
            
            AddCharacter(vertices, charIt->second, glm::vec2(offsetX, offsetY), base, color, transform);
            
            vertices += 4;
            numCharacters++;
            
            maxLineHeight = glm::max(charIt->second.height, maxLineHeight);
            offsetX += charIt->second.xAdvance;
            
            if (i<wideString.length()-1)
            {
                std::map<std::pair<uint32_t, uint32_t>, float>::iterator kerningIt = kerning.find(std::pair<uint32_t, uint32_t>(wideString[i], wideString[i+1]));
                
                if (kerningIt != kerning.end())
                    offsetX += kerningIt->second;
            }
        }
    }
    
    maxLineLength = glm::max(offsetX, maxLineLength);
    
    if (measuredSize)
    {
        *measuredSize = glm::vec2(maxLineLength, glm::abs(offsetY - maxLineHeight));
    }
    
    return numCharacters*4;
}

void Font::AddCharacter(Vertex_P3_C4_UV* buffer, const Font::Character& character, glm::vec2 position, float baseline, glm::vec4 color, const glm::mat4x4& transform)
{
    float xOffset = character.xOffset;
    float yOffset = -character.yOffset + baseline;
    
    glm::vec4 points[4];
    
    points[0] = glm::vec4(position.x + xOffset, position.y + yOffset - character.height, 0.f, 1.f);
    points[1] = glm::vec4(position.x + xOffset, position.y + yOffset, 0.f, 1.f);
    points[2] = glm::vec4(position.x + character.width + xOffset, position.y + yOffset, 0.f, 1.f);
    points[3] = glm::vec4(position.x + character.width + xOffset, position.y + yOffset - character.height, 0.f, 1.f);
    
    points[0] = transform * points[0];
    points[1] = transform * points[1];
    points[2] = transform * points[2];
    points[3] = transform * points[3];
    
    buffer[0].position[0] = points[0].x;
    buffer[0].position[1] = points[0].y;
    buffer[0].position[2] = points[0].z;
    buffer[0].color[0] = color.r;
    buffer[0].color[1] = color.g;
    buffer[0].color[2] = color.b;
    buffer[0].color[3] = color.a;
    buffer[0].uv[0] = character.uvx;
    buffer[0].uv[1] = character.uvy + character.uvv;
    buffer[1].position[0] = points[1].x;
    buffer[1].position[1] = points[1].y;
    buffer[1].position[2] = points[1].z;
    buffer[1].color[0] = color.r;
    buffer[1].color[1] = color.g;
    buffer[1].color[2] = color.b;
    buffer[1].color[3] = color.a;
    buffer[1].uv[0] = character.uvx;
    buffer[1].uv[1] = character.uvy;
    buffer[2].position[0] = points[2].x;
    buffer[2].position[1] = points[2].y;
    buffer[2].position[2] = points[2].z;
    buffer[2].color[0] = color.r;
    buffer[2].color[1] = color.g;
    buffer[2].color[2] = color.b;
    buffer[2].color[3] = color.a;
    buffer[2].uv[0] = character.uvx + character.uvu;
    buffer[2].uv[1] = character.uvy;
    buffer[3].position[0] = points[3].x;
    buffer[3].position[1] = points[3].y;
    buffer[3].position[2] = points[3].z;
    buffer[3].color[0] = color.r;
    buffer[3].color[1] = color.g;
    buffer[3].color[2] = color.b;
    buffer[3].color[3] = color.a;
    buffer[3].uv[0] = character.uvx + character.uvu;
    buffer[3].uv[1] = character.uvy + character.uvv;
}

FontRenderer::FontRenderer(int maxCharacters)
    : _MaxCharacters(maxCharacters)
{
    PbAssert(!_Instance);
    
    _Instance = this;
    
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _MaxCharacters*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatDynamic, kVertexFormat_P3_C4_UV, _MaxCharacters*4);
    
    _IndexBuffer->Lock();
    
    unsigned short* indices = _IndexBuffer->GetData();
    for (int i=0; i<_MaxCharacters; i++)
    {
        indices[0] = (i*4)+0;
        indices[1] = (i*4)+1;
        indices[2] = (i*4)+2;
        indices[3] = (i*4)+0;
        indices[4] = (i*4)+2;
        indices[5] = (i*4)+3;
        indices += 6;
    }
    
    _IndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(FontRenderable::GetStaticType(), this);
}

FontRenderer::~FontRenderer()
{
    _Instance = 0;
    
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
}

FontRenderer* FontRenderer::Instance()
{
    return _Instance;
}

Font* FontRenderer::LoadFont(const std::string& name, const std::string& filename, bool createMips, bool hasPremultipliedAlpha)
{
    FontMap::iterator it = _Fonts.find(name);
    
    if (it != _Fonts.end())
        return it->second;
    
    Font* font = new Font();
    
    font->texture = 0;
    
    std::string modifier;
    
    if (GraphicsDevice::Instance()->GetDisplayDensity() >= 64.f)
    {
        modifier = "-hdr";
    }
    else if (GraphicsDevice::Instance()->GetDisplayDensity() >= 32.f)
    {
        modifier = "-hd";
    }

    std::string fntFilename = filename + modifier + ".fnt";
    auto file = pb::FileSystem::Instance()->OpenFile(fntFilename);
    
    std::string fontContents;
    
    if (file)
    {
        file->ReadAll(fontContents);
    }
    
    std::vector<std::string> lines;
    SplitString(fontContents, '\n', lines);
    
    glm::vec2 texSize;
    
    for (std::vector<std::string>::iterator line = lines.begin(); line != lines.end(); ++line)
    {
        std::vector<std::string> elements;
        SplitString(*line, ' ', elements);
        
        if (elements.size() < 1)
            continue;
        
        std::map<std::string, std::string> data;
        for (std::vector<std::string>::iterator element = elements.begin(); element != elements.end(); ++element)
        {
            data[element->substr(0, element->find('='))] = element->substr(element->find('=')+1);
        }
        
        std::string elementType = elements[0];
        
        if (elementType == "info")
        {
            int size = (int)strtol(data["size"].c_str(), 0, 10);
            
            font->sourceSize = size;
        } else if (elementType == "common")
        {
            int lineHeight = (int)strtol(data["lineHeight"].c_str(), 0, 10);
            int base = (int)strtol(data["base"].c_str(), 0, 10);
            int scaleW = (int)strtol(data["scaleW"].c_str(), 0, 10);
            int scaleH = (int)strtol(data["scaleH"].c_str(), 0, 10);
            
            font->base = (float)base/font->sourceSize;
            font->lineHeight = (float)lineHeight/font->sourceSize;
            texSize = glm::vec2(scaleW, scaleH);
        } else if (elementType == "page")
        {
            std::string texFilename = "/fonts/" + data["file"].substr(1, data["file"].find('"', 1)-1);
            font->texture = GraphicsDevice::Instance()->CreateTexture();
            font->texture->LoadFromFile(texFilename, createMips);
        } else if (elementType == "char")
        {
            Font::Character character;
            
            uint32_t charCode = strtol(data["id"].c_str(), 0, 10);
            float x = strtol(data["x"].c_str(), 0, 10);
            float y = strtol(data["y"].c_str(), 0, 10);
            float width = strtol(data["width"].c_str(), 0, 10);
            float height = strtol(data["height"].c_str(), 0, 10);
            float xoffset = strtol(data["xoffset"].c_str(), 0, 10);
            float yoffset = strtol(data["yoffset"].c_str(), 0, 10);
            float xadvance = strtol(data["xadvance"].c_str(), 0, 10);
            
            character.width = width/(float)font->sourceSize;
            character.height = height/(float)font->sourceSize;
            character.uvx = x/texSize[0];
            character.uvy = y/texSize[1];
            character.uvu = width/texSize[0];
            character.uvv = height/texSize[1];
            character.xOffset = xoffset/(float)font->sourceSize;
            character.yOffset = yoffset/(float)font->sourceSize;
            character.xAdvance = xadvance/(float)font->sourceSize;
            
            font->chars[charCode] = character;
        } else if (elementType == "kerning")
        {
            uint32_t charOne = strtol(data["first"].c_str(), 0, 10);
            uint32_t charTwo = strtol(data["second"].c_str(), 0, 10);
            int amount = (int)strtol(data["amount"].c_str(), 0, 10);
            
            font->kerning[std::pair<uint32_t, uint32_t>(charOne, charTwo)] = amount/texSize[0];
        }
    }
    
    _Fonts[name] = font;
    
    return font;
}

Font* FontRenderer::GetFont(const std::string& name)
{
    FontMap::iterator it = _Fonts.find(name);
    
    if (it != _Fonts.end())
        return it->second;
    
    return 0;
}

void FontRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    Shader* shader = renderables[0]->GetShader();
    if (!shader)
        return;
    
    ShaderTechnique* technique = shader->GetTechnique(renderScheme);
    
    if (!technique)
        return;
    
    ShaderPass* shaderPass = technique->GetPass(0);
    shaderPass->Bind();
    shaderPass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
    
    for (int i=0; i<count; i++)
    {
        FontRenderable& renderable = *static_cast<FontRenderable*>(renderables[i]);

        Font* font;
        
        FontMap::iterator fontIt = _Fonts.find(renderable.Font);
        if (fontIt == _Fonts.end() || fontIt->second->texture == 0 || renderable.Text.length() == 0)
        {
            continue;
        }
        
        if (renderable.Text.length() > _MaxCharacters)
        {
            PbLogDebug("pb.renderer.font", "String (%s) is too long for the MaxCharacters value set\n", renderable.Text.c_str());
            continue;
        }
        
        font = fontIt->second;
        
        _VertexBuffer->Lock();
        pb::Vertex_P3_C4_UV* vertices = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
        int numElements = font->FillVertices(vertices, renderable.Text, _VertexBuffer->GetMaxSize());
        _VertexBuffer->Unlock(numElements);
        
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->BindTexture(0, font->texture);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", renderable.Tint);
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
        
        if (_VertexBuffer->GetCurrentSize())
            GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (_VertexBuffer->GetCurrentSize()/4)*6);
    }
    
    GraphicsDevice::Instance()->BindTexture(0, 0);
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
}

float FontRenderer::FitString(glm::vec2 region, const std::string& name, const std::string& font, float preferredSize)
{
    glm::vec2 size = MeasureString(name, font, preferredSize);
    
    float x = glm::max(glm::max(region.x, size.x)/region.x, 1.f);
    float y = glm::max(glm::max(region.y, size.y)/region.y, 1.f);
    
    return preferredSize / glm::max(x, y);
}

glm::vec2 FontRenderer::MeasureString(const std::string& name, const std::string& string, float size)
{
    Font* font;
    
    FontMap::iterator fontIt = _Fonts.find(name);
    
    if (fontIt == _Fonts.end() || fontIt->second->texture == 0)
        return glm::vec2(0.f, 0.f);
    
    font = fontIt->second;
    
    float maxLineHeight = 0.f;
    float maxLineLength = 0.f;
    float offsetX = 0.f;
    float offsetY = 0.f;
    
    wstring wideString = pb::UTF8toUTF32(string);
    
    for (int i=0; i<wideString.length(); i++)
    {
        if (wideString[i] == '\n')
        {
            maxLineHeight = 0.f;
            maxLineLength = glm::max(offsetX, maxLineLength);
            offsetX = 0.f;
            offsetY -= 1.f;
        } else {
            std::map<uint32_t, Font::Character>::iterator charIt = font->chars.find(wideString[i]);
            
            if (charIt == font->chars.end())
                continue;

            maxLineHeight = glm::max(charIt->second.height, maxLineHeight);
            
            offsetX += charIt->second.xAdvance;
            
            if (i<string.length()-1)
            {
                std::map<std::pair<uint32_t, uint32_t>, float>::iterator kerningIt = font->kerning.find(std::pair<uint32_t, uint32_t>(wideString[i], wideString[i+1]));
                
                if (kerningIt != font->kerning.end())
                    offsetX += kerningIt->second;
            }
        }
    }
    
    maxLineLength = glm::max(maxLineLength, offsetX);
    
    return glm::vec2(maxLineLength * size, glm::abs((offsetY-maxLineHeight) * size));
}

void FontRenderer::SplitString(const std::string& string, char seperator, std::vector<std::string>& output)
{
    std::string item;
    
    for (int i=0; i<string.length(); i++)
    {
        char ch=string[i];
        
        if (ch == seperator)
        {
            if (!item.empty())
            {
                output.push_back(item);
            }
            item = "";
        }
        else
        {
            item += ch;
        }
    }
    if (!item.empty())
    {
        output.push_back(item);
    }
}
