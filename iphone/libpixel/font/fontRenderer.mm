#include "libpixel/graphics/render/font/fontRenderer.h"

#include "iphonefont/font.h"

namespace libpixel
{

FontRenderer::FontRenderer()
{
    
}

FontRenderer::~FontRenderer()
{
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
}

void FontRenderer::Render()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    for (InstanceList::iterator it = _Instances.begin(); it != _Instances.end(); ++it)
    {
        Font* font;
        
        FontMap::iterator fontIt = _Fonts.find(it->_Font);
        if (fontIt != _Fonts.end())
        {
            font = fontIt->second;
        } else {
            font = new Font(it->_Font);
            _Fonts[it->_Font] = font;
        }
        
        int align = 0;
        
        switch (it->_Alignment) {
            case kFontAlignLeft:
                align = -1;
                break;
            case kFontAlignCenter:
                align = 0;
                break;
            case kFontAlignRight:
                align = 1;
                break;
        }
        
        font->print(it->_String, it->_Position[0], it->_Position[1], 0.f, align, it->_Scale / 32.f);
    }
    
    _Instances.clear();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

bool FontRenderer::AttachToRenderer(const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment, float rotation, float scale)
{
    FontInstance instance;
    
    instance._Font = fontName;
    instance._String = string;
    instance._Position = position;
    instance._Alignment = alignment;
    instance._Rotation = rotation;
    instance._Scale = scale / 2.f;
    
    _Instances.push_back(instance);
    
    Render();
    
    return true;
}

}