#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"

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

Uid FontRenderable::GetRenderableType()
{
    return TypeHash("font");
}

void FontRenderable::CalculateMVP(Viewport* viewport)
{
    _MVPMatrix = glm::scale(glm::mat4x4(), glm::vec3(Size, Size, 1));
    _MVPMatrix = glm::translate(_MVPMatrix, glm::vec3(Offset, 0, 0));
    _MVPMatrix = Transform * _MVPMatrix;
    _MVPMatrix = viewport->GetCamera()->ViewProjectionMatrix * _MVPMatrix;
}

Effect* FontRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/sprite.fx");
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
    Offset = Game::Instance()->GetFontRenderer()->MeasureString(Font, Text, Size);
    
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
}

float Font::FillVertexBuffer(VertexBuffer* vertexBuffer, const std::string& string)
{
    vertexBuffer->Lock();
    
    Vertex_PXYZ_UV* vertices = static_cast<Vertex_PXYZ_UV*>(vertexBuffer->GetData());
    
    int numCharacters = 0;        
    
    float offset = 0.f;
    for (int i=0; i<string.length(); i++)
    {
        std::map<char, Font::Character>::iterator charIt = chars.find(string[i]);
        
        if (charIt == chars.end())
            continue;
        
        AddCharacter(vertices, charIt->second, offset, base);
        
        vertices += 4;
        numCharacters++;
        
        offset += charIt->second.xAdvance;
        
        if (i<string.length()-1)
        {
            std::map<std::pair<char, char>, float>::iterator kerningIt = kerning.find(std::pair<char, char>(string[i], string[i+1]));
            
            if (kerningIt != kerning.end())
                offset += kerningIt->second;
        }
    }
    
    vertexBuffer->Unlock(numCharacters*4);
    
    return offset;
}

void Font::AddCharacter(Vertex_PXYZ_UV* buffer, const Font::Character& character, float offset, float baseline)
{
    float xOffset = character.xOffset;
    float yOffset = -character.yOffset + baseline;
    
    buffer[0].position[0] = offset + xOffset;
    buffer[0].position[1] = yOffset - character.height;
    buffer[0].position[2] = 0.f;
    buffer[0].uv[0] = character.uvx;
    buffer[0].uv[1] = character.uvy + character.uvv;
    buffer[1].position[0] = offset + xOffset;
    buffer[1].position[1] = yOffset;
    buffer[1].position[2] = 0.f;
    buffer[1].uv[0] = character.uvx;
    buffer[1].uv[1] = character.uvy;
    buffer[2].position[0] = offset + character.width + xOffset;
    buffer[2].position[1] = yOffset;
    buffer[2].position[2] = 0.f;
    buffer[2].uv[0] = character.uvx + character.uvu;
    buffer[2].uv[1] = character.uvy;
    buffer[3].position[0] = offset + character.width + xOffset;
    buffer[3].position[1] = yOffset - character.height;
    buffer[3].position[2] = 0.f;
    buffer[3].uv[0] = character.uvx + character.uvu;
    buffer[3].uv[1] = character.uvy + character.uvv;
}

FontRenderer::FontRenderer(int maxCharacters)
    : _MaxCharacters(maxCharacters)
{
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _MaxCharacters*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatDynamic, kVertexFormat_P_XYZ_UV, _MaxCharacters*4);
    
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
    
    Renderer::Instance()->SetHandler(TypeHash("font"), this);
    
    Renderer::Instance()->GetEffectManager()->LoadEffect("/default/effects/sprite.fx");
}

FontRenderer::~FontRenderer()
{
    Renderer::Instance()->GetEffectManager()->UnloadEffect("/default/effects/sprite.fx");
    
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
}

Font* FontRenderer::LoadFont(const std::string& name, bool createMips)
{
    FontMap::iterator it = _Fonts.find(name);
    
    if (it != _Fonts.end())
        return it->second;
    
    Font* font = new Font();
    
    font->texture = 0;
    
    std::string fileRoot = FileHelpers::GetRootPath();
    
    std::string fntFilename = fileRoot + "/data/fonts/" + name + (ScreenHelpers::IsHighResolution() ? "-hd" : "") + ".fnt";
    
    std::string fontContents = FileHelpers::FileToString(fntFilename);
    
    std::vector<std::string> lines;
    SplitString(fontContents, '\n', lines);
    
    glm::vec2 texSize;
    
    for (std::vector<std::string>::iterator line = lines.begin(); line != lines.end(); ++line)
    {
        std::vector<std::string> elements;
        SplitString(*line, ' ', elements);
        
        if (elements.size() < 1 )
            continue;
        
        std::map<std::string, std::string> data;
        for (std::vector<std::string>::iterator element = elements.begin(); element != elements.end(); ++element)
        {
            data[element->substr(0, element->find('='))] = element->substr(element->find('=')+1);
        }
        
        std::string elementType = elements[0];
        
        if (elementType == "info")
        {
            int size = (int)atoi(data["size"].c_str());
            
            font->size = size;
        } else if (elementType == "common")
        {
            int lineHeight = (int)atoi(data["lineHeight"].c_str());
            int base = (int)atoi(data["base"].c_str());
            int scaleW = (int)atoi(data["scaleW"].c_str());
            int scaleH = (int)atoi(data["scaleH"].c_str());
            
            font->base = (float)base/(float)font->size;
            font->lineHeight = (float)lineHeight / (float)scaleH;
            texSize = glm::vec2(scaleW, scaleH);
        } else if (elementType == "page")
        {
            std::string texFilename = fileRoot + "/data/fonts/" + data["file"].substr(1, data["file"].find('"', 1)-1);
            font->texture = GraphicsDevice::Instance()->CreateTexture();
            font->texture->LoadFromPng(texFilename, createMips);
        } else if (elementType == "char")
        {
            Font::Character character;
            
            char charCode = (char)atoi(data["id"].c_str());
            float x = atoi(data["x"].c_str());
            float y = atoi(data["y"].c_str());
            float width = atoi(data["width"].c_str());
            float height = atoi(data["height"].c_str());
            float xoffset = atoi(data["xoffset"].c_str());
            float yoffset = atoi(data["yoffset"].c_str());
            float xadvance = atoi(data["xadvance"].c_str());
            
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
            char charOne = (char)atoi(data["first"].c_str());
            char charTwo = (char)atoi(data["second"].c_str());
            int amount = (int)atoi(data["amount"].c_str());
            
            font->kerning[std::pair<char, char>(charOne, charTwo)] = amount/texSize[0];
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

void FontRenderer::Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
#ifndef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
#else
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
#endif
    
    GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
    
    for (int i=0; i<count; i++)
    {
        FontRenderable& renderable = *static_cast<FontRenderable*>(renderables[i]);

        Font* font;
        
        FontMap::iterator fontIt = _Fonts.find(renderable.Font);
        if (fontIt == _Fonts.end() || fontIt->second->texture == 0)
        {
            continue;
        }
        
        if (renderable.Text.length() > _MaxCharacters)
        {
            printf("String (%s) is too long for the MaxCharacters value set\n", renderable.Text.c_str());
            continue;
        }
        
        font = fontIt->second;
        
        font->FillVertexBuffer(_VertexBuffer, renderable.Text);
        
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->BindTexture(font->texture);
        
        effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", renderable.GetMVP());
        effectPass->GetShaderProgram()->SetUniform("diffuseColor", renderable.Tint);
        effectPass->GetShaderProgram()->SetUniform("diffuseTexture", 0);
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, (_VertexBuffer->GetCurrentSize()/4)*6);
    }
    
    GraphicsDevice::Instance()->BindTexture(0);
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
}

float FontRenderer::MeasureString(const std::string& name, const std::string& string, float size)
{
    Font* font;
    
    FontMap::iterator fontIt = _Fonts.find(name);
    
    if (fontIt == _Fonts.end() || fontIt->second->texture == 0)
        return 0.f;
    
    font = fontIt->second;
    
    float offset = 0.f;
    for (int i=0; i<string.length(); i++)
    {
        std::map<char, Font::Character>::iterator charIt = font->chars.find(string[i]);
        
        if (charIt == font->chars.end())
            continue;
        
        offset += charIt->second.xAdvance;
        
        if (i<string.length()-1)
        {
            std::map<std::pair<char, char>, float>::iterator kerningIt = font->kerning.find(std::pair<char, char>(string[i], string[i+1]));
            
            if (kerningIt != font->kerning.end())
                offset += kerningIt->second;
        }
    }
    
    return offset * size;
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
