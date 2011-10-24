#include "libpixel/graphics/device/texture.h"
#include "libpixel/graphics/render/font/fontRenderer.h"

libpixel::FontRenderer::Font::Font()
{
    
}

libpixel::FontRenderer::Font::~Font()
{
    delete texture;
}

libpixel::FontRenderer::FontRenderer()
{
    
}

libpixel::FontRenderer::~FontRenderer()
{
    for (FontMap::iterator it = _Fonts.begin(); it != _Fonts.end(); ++it)
    {
        delete it->second;
    }
}

void libpixel::FontRenderer::Render()
{
    
}

bool libpixel::FontRenderer::AttachToRenderer(const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment, float rotation, float scale)
{
    return false;
}
