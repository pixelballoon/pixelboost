#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"
#include "pixelboost/util/localisation/string.h"

using namespace pb;

FontRenderable::FontRenderable(Uid entityId)
    : Renderable(entityId)
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
    glm::vec2 size = Engine::Instance()->GetFontRenderer()->MeasureString(Font, Text, Size);
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
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/data/shaders/pb_textured.shc");
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
    Offset = Engine::Instance()->GetFontRenderer()->MeasureString(Font, Text, 1.f).x;
    
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

glm::vec2 Font::FillVertexBuffer(VertexBuffer* vertexBuffer, const std::string& string)
{
    vertexBuffer->Lock();
    
    Vertex_P3_UV* vertices = static_cast<Vertex_P3_UV*>(vertexBuffer->GetData());
    
    wstring wideString = UTF8toUTF32(string);
    
    int numCharacters = 0;
    
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
            
            AddCharacter(vertices, charIt->second, glm::vec2(offsetX, offsetY), base);
            
            vertices += 4;
            numCharacters++;
            
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
    
    vertexBuffer->Unlock(numCharacters*4);
    
    return glm::vec2(maxLineLength, glm::abs(offsetY - 1.f));
}

void Font::AddCharacter(Vertex_P3_UV* buffer, const Font::Character& character, glm::vec2 position, float baseline)
{
    float xOffset = character.xOffset;
    float yOffset = -character.yOffset + baseline;
    
    buffer[0].position[0] = position.x + xOffset;
    buffer[0].position[1] = position.y + yOffset - character.height;
    buffer[0].position[2] = 0.f;
    buffer[0].uv[0] = character.uvx;
    buffer[0].uv[1] = character.uvy + character.uvv;
    buffer[1].position[0] = position.x + xOffset;
    buffer[1].position[1] = position.y + yOffset;
    buffer[1].position[2] = 0.f;
    buffer[1].uv[0] = character.uvx;
    buffer[1].uv[1] = character.uvy;
    buffer[2].position[0] = position.x + character.width + xOffset;
    buffer[2].position[1] = position.y + yOffset;
    buffer[2].position[2] = 0.f;
    buffer[2].uv[0] = character.uvx + character.uvu;
    buffer[2].uv[1] = character.uvy;
    buffer[3].position[0] = position.x + character.width + xOffset;
    buffer[3].position[1] = position.y + yOffset - character.height;
    buffer[3].position[2] = 0.f;
    buffer[3].uv[0] = character.uvx + character.uvu;
    buffer[3].uv[1] = character.uvy + character.uvv;
}

FontRenderer::FontRenderer(int maxCharacters)
    : _MaxCharacters(maxCharacters)
{
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _MaxCharacters*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatDynamic, kVertexFormat_P3_UV, _MaxCharacters*4);
    
    _IndexBuffer->Lock();
    
    unsigned short* indices = _IndexBuffer->GetData();
    for (int i=0; i<_MaxCharacters; i++)
    {
        indices[0] = (i*4)+2;
        indices[1] = (i*4)+1;
        indices[2] = (i*4)+0;
        indices[3] = (i*4)+3;
        indices[4] = (i*4)+2;
        indices[5] = (i*4)+0;
        indices += 6;
    }
    
    _IndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(FontRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/data/shaders/pb_textured.shc");
}

FontRenderer::~FontRenderer()
{
    Renderer::Instance()->GetShaderManager()->UnloadShader("/data/shaders/pb_textured.shc");
    
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
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
    pb::File* file = pb::FileSystem::Instance()->OpenFile(fntFilename);
    
    std::string fontContents;
    
    if (file)
    {
        file->ReadAll(fontContents);
        delete file;
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
            
            font->size = size;
        } else if (elementType == "common")
        {
            int lineHeight = (int)strtol(data["lineHeight"].c_str(), 0, 10);
            int base = (int)strtol(data["base"].c_str(), 0, 10);
            int scaleW = (int)strtol(data["scaleW"].c_str(), 0, 10);
            int scaleH = (int)strtol(data["scaleH"].c_str(), 0, 10);
            
            font->base = (float)base/(float)font->size;
            font->lineHeight = (float)lineHeight / (float)scaleH;
            texSize = glm::vec2(scaleW, scaleH);
        } else if (elementType == "page")
        {
            std::string texFilename = "/data/fonts/" + data["file"].substr(1, data["file"].find('"', 1)-1);
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
            
            character.width = width/(float)font->size;
            character.height = height/(float)font->size;
            character.uvx = x/texSize[0];
            character.uvy = y/texSize[1];
            character.uvu = width/texSize[0];
            character.uvv = height/texSize[1];
            character.xOffset = xoffset/(float)font->size;
            character.yOffset = yoffset/(float)font->size;
            character.xAdvance = xadvance/(float)font->size;
            
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

void FontRenderer::Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
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
            wprintf(L"String (%s) is too long for the MaxCharacters value set\n", renderable.Text.c_str());
            continue;
        }
        
        font = fontIt->second;
        
        font->FillVertexBuffer(_VertexBuffer, renderable.Text);
        
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->BindTexture(font->texture);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseColor", renderable.Tint);
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
        
        if (_VertexBuffer->GetCurrentSize())
            GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (_VertexBuffer->GetCurrentSize()/4)*6);
    }
    
    GraphicsDevice::Instance()->BindTexture(0);
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
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
    
    float maxLineLength = 0.f;
    float offsetX = 0.f;
    float offsetY = 0.f;
    
    wstring wideString = pb::UTF8toUTF32(string);
    
    for (int i=0; i<wideString.length(); i++)
    {
        if (wideString[i] == '\n')
        {
            maxLineLength = glm::max(maxLineLength, offsetX);
            offsetX = 0.f;
            offsetY -= 1.f;
        } else {
            std::map<uint32_t, Font::Character>::iterator charIt = font->chars.find(wideString[i]);
            
            if (charIt == font->chars.end())
                continue;
            
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
    
    return glm::vec2(maxLineLength * size, glm::abs((offsetY-2.f) * size));
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

#endif
