#include "pixelboost/file/fileHelpers.h"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/texture.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/helper/screenHelpers.h"
#include "pixelboost/graphics/render/font/fontRenderer.h"

using namespace pixelboost;

FontRenderer::FontRenderer(int maxCharacters)
    : _MaxCharacters(maxCharacters)
{
    _IndexBuffer = GraphicsDevice::Instance()->CreateIndexBuffer(kBufferFormatStatic, _MaxCharacters*6);
    _VertexBuffer = GraphicsDevice::Instance()->CreateVertexBuffer(kBufferFormatStatic, kVertexFormat_P_XYZ_UV, _MaxCharacters*4);
    
    _IndexBuffer->Lock();
    
    unsigned short* indices = _IndexBuffer->GetData();
    for (int i=0; i<_MaxCharacters; i++)
    {
        indices[0] = (i*4);
        indices[1] = (i*4)+1;
        indices[2] = (i*4)+2;
        indices[3] = (i*4);
        indices[4] = (i*4)+2;
        indices[5] = (i*4)+3;
        indices += 6;
    }
    
    _IndexBuffer->Unlock();
}

FontRenderer::~FontRenderer()
{
    GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
}

void FontRenderer::AddCharacter(Vertex_PXYZ_UV* buffer, const Font::Character& character, float offset, float baseline)
{
    float xOffset = character.xOffset;
    float yOffset = -character.yOffset + baseline;
    
    buffer[0].position[0] = offset + xOffset;
    buffer[0].position[1] = yOffset - character.height;
    buffer[0].uv[0] = character.x;
    buffer[0].uv[1] = character.y + character.height;
    buffer[1].position[0] = offset + xOffset;
    buffer[1].position[1] = yOffset;
    buffer[1].uv[0] = character.x;
    buffer[1].uv[1] = character.y;
    buffer[2].position[0] = offset + character.width + xOffset;
    buffer[2].position[1] = yOffset;
    buffer[2].uv[0] = character.x + character.width;
    buffer[2].uv[1] = character.y;
    buffer[3].position[0] = offset + character.width + xOffset;
    buffer[3].position[1] = yOffset - character.height;
    buffer[3].uv[0] = character.x + character.width;
    buffer[3].uv[1] = character.y + character.height;
}

void FontRenderer::LoadFont(const std::string& name)
{
    Font* font = new Font();
    
    std::string fileRoot = FileHelpers::GetRootPath();
    
    std::string fntFilename = fileRoot + "/data/fonts/" + name + (ScreenHelpers::IsHighResolution() ? "-hd" : "") + ".fnt";
    
    std::string fontContents = FileHelpers::FileToString(fntFilename);
    
    std::vector<std::string> lines;
    SplitString(fontContents, '\n', lines);
    
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
        
        Vec2 texSize;
        
        if (elementType == "info")
        {
            
        } else if (elementType == "common")
        {
            int lineHeight = (int)atoi(data["lineHeight"].c_str());
            int base = (int)atoi(data["base"].c_str());
            int scaleW = (int)atoi(data["scaleW"].c_str());
            int scaleH = (int)atoi(data["scaleH"].c_str());
            
            font->base = (float)base / (float)scaleH;
            font->lineHeight = (float)lineHeight / (float)scaleH;
            texSize = Vec2(scaleW, scaleH);
        } else if (elementType == "page")
        {
            std::string texFilename = fileRoot + "/data/fonts/" + data["file"].substr(1, data["file"].find('"', 1)-1);
            font->texture = GraphicsDevice::Instance()->CreateTexture();
            font->texture->Load(texFilename, true);
        } else if (elementType == "char")
        {
            Font::Character character;
            
            char charCode = (char)atoi(data["id"].c_str());
            int x = (int)atoi(data["x"].c_str());
            int y = (int)atoi(data["y"].c_str());
            int width = (int)atoi(data["width"].c_str());
            int height = (int)atoi(data["height"].c_str());
            int xoffset = (int)atoi(data["xoffset"].c_str());
            int yoffset = (int)atoi(data["yoffset"].c_str());
            int xadvance = (int)atoi(data["xadvance"].c_str());
            
            character.x = x/texSize[0];
            character.y = y/texSize[1];
            character.width = width/texSize[0];
            character.height = height/texSize[1];
            character.xOffset = xoffset/texSize[0];
            character.yOffset = yoffset/texSize[1];
            character.xAdvance = xadvance/texSize[0];
            
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
}

void FontRenderer::Update(float time)
{
    _Instances.clear();
}

void FontRenderer::Render(RenderLayer* layer)
{
    InstanceList& instances = _Instances[layer];
    
    if (instances.size() == 0)
        return;
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    
#ifndef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
    
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    _IndexBuffer->Bind();
    
    for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it)
    {
        Font* font;
        
        FontMap::iterator fontIt = _Fonts.find(it->_Font);
        if (fontIt == _Fonts.end())
            continue;
        
        font = fontIt->second;
        
        _VertexBuffer->Lock();
        
        Vertex_PXYZ_UV* vertexBuffer = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
        
        float offset = 0.f;
        for (int i=0; i<it->_String.length(); i++)
        {
            std::map<char, Font::Character>::iterator charIt = font->chars.find(it->_String[i]);
            
            if (charIt == font->chars.end())
                continue;
            
            AddCharacter(vertexBuffer, charIt->second, offset, font->base);
            
            vertexBuffer += 4;
            
            offset += charIt->second.xAdvance;
            
            if (i<it->_String.length()-1)
            {
                std::map<std::pair<char, char>, float>::iterator kerningIt = font->kerning.find(std::pair<char, char>(it->_String[i], it->_String[i+1]));
                
                if (kerningIt != font->kerning.end())
                    offset += kerningIt->second;
            }
        }
        
        _VertexBuffer->Unlock();
        
        _VertexBuffer->Bind();
        
        font->texture->Bind(0);
        
        glPushMatrix();
        
        glTranslatef(it->_Position[0], it->_Position[1], 0.f);

        glRotatef(it->_Rotation, 0, 0, 1);
        
        glScalef(it->_Scale, it->_Scale, 1.f);
        
        switch (it->_Alignment) {
            case kFontAlignLeft:
                break;
            case kFontAlignCenter:
                glTranslatef(-offset/2.f, 0, 0);
                break;
            case kFontAlignRight:
                glTranslatef(-offset, 0, 0);
                break;
        }
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, it->_String.length()*6);
        
        glPopMatrix();
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

bool FontRenderer::AttachToRenderer(RenderLayer* layer, const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment, float scale, float rotation, Vec4 color)
{
    FontInstance instance;
    
    instance._Font = fontName;
    instance._String = string;
    instance._Position = position;
    instance._Alignment = alignment;
    instance._Rotation = rotation;
    instance._Scale = scale;
    instance._Color = color;
    
    _Instances[layer].push_back(instance);
    
    return true;
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
