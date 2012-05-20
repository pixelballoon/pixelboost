#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifndef PIXELBOOST_DISABLE_GWEN

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/render/font/fontRenderer.h"
#include "pixelboost/graphics/render/gwen/gwenRenderer.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"
#include "pixelboost/graphics/render/sprite/sprite.h"
#include "pixelboost/graphics/render/sprite/spriteRenderer.h"
#include "pixelboost/logic/game.h"

#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"

#include <math.h>

using namespace pb;

GwenRenderer::GwenRenderer(pb::RenderLayer* layer)
    : _Layer(layer)
{
    
}

GwenRenderer::~GwenRenderer()
{

}

void GwenRenderer::Init()
{
    
}

void GwenRenderer::Begin()
{

}

void GwenRenderer::End()
{
    
}

void GwenRenderer::SetDrawColor(Gwen::Color colour)
{
    _Colour = glm::vec4(colour.r, colour.g, colour.b, colour.a);
}

void GwenRenderer::DrawFilledRect( Gwen::Rect rect )
{
    Translate(rect);
    
    pb::Game::Instance()->GetPrimitiveRenderer()->AttachBox(_Layer, Vec2(rect.x+rect.w/2, -rect.y-rect.h/2)/32.f, Vec2(rect.w, rect.h)/32.f, Vec3(0,0,0), Vec4(_Colour.r, _Colour.g, _Colour.b, _Colour.a));
}

void GwenRenderer::StartClip()
{
    // TODO: Implement clipping
}

void GwenRenderer::EndClip()
{

}

void GwenRenderer::DrawTexturedRect(Gwen::Texture* definition, Gwen::Rect rect, float u1, float v1, float u2, float v2)
{
    Translate(rect);
    pb::Game::Instance()->GetSpriteRenderer()->AttachCustom(_Layer, definition->name.c_str(), Vec2(rect.w+rect.x, -rect.y)/32.f, Vec2(rect.w, rect.h)/32.f, Vec4(u1, v1, u2, v2), Vec3(0,0,0));
}

void GwenRenderer::LoadTexture(Gwen::Texture* definition)
{
    std::shared_ptr<SpriteSheet> sheet = pb::Game::Instance()->GetSpriteRenderer()->CreateSpriteSheet(definition->name.c_str());
    
    pb::Texture* texture = sheet->LoadTexture(pb::FileHelpers::GetRootPath() + "/data/" + definition->name.c_str());

    definition->width = texture->GetSize()[0];
    definition->height = texture->GetSize()[1];
}

void GwenRenderer::FreeTexture(Gwen::Texture* definition)
{
    pb::Game::Instance()->GetSpriteRenderer()->UnloadSpriteSheet(definition->name.c_str());
}

Gwen::Color GwenRenderer::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default )
{
    // TODO: Properly implement this - it's currently taken from the OpenGL sample code, and isn't particularly nice
    
    std::shared_ptr<SpriteSheet> sheet = pb::Game::Instance()->GetSpriteRenderer()->GetSpriteSheet(pTexture->name.c_str());
    
    sheet->_Texture->Bind();
        
    if (!pTexture->width)
        return col_default;
    
    unsigned int iPixelSize = sizeof(unsigned char) * 4;
    
    unsigned char* data = (unsigned char*) malloc( iPixelSize * pTexture->width * pTexture->height );
    
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    unsigned int iOffset = (y * pTexture->width + x) * 4;
 
    Gwen::Color c;
    c.r = data[0 + iOffset];
    c.g = data[1 + iOffset];
    c.b = data[2 + iOffset];
    c.a = data[3 + iOffset];
    
    free( data );

    return c;
}

void GwenRenderer::RenderText(Gwen::Font* font, Gwen::Point pos, const Gwen::UnicodeString& text)
{
    Translate(pos.x, pos.y);
    
    Gwen::String fontName = Gwen::Utility::UnicodeToString(font->facename);
    Gwen::String convertedString = Gwen::Utility::UnicodeToString(text);
    
    pb::Game::Instance()->GetFontRenderer()->LoadFont(fontName);
    
    float size = font->size * Scale();    
    if (!text.length())
        return;
        
    pb::Game::Instance()->GetFontRenderer()->AttachToRenderer(_Layer, fontName, convertedString, Vec2(pos.x, -pos.y - size)/32.f, kFontAlignLeft, size/32.f);
}

Gwen::Point GwenRenderer::MeasureText(Gwen::Font* font, const Gwen::UnicodeString& text)
{
    Gwen::String fontName = Gwen::Utility::UnicodeToString(font->facename);
    Gwen::String convertedString = Gwen::Utility::UnicodeToString(text);
    
    pb::Game::Instance()->GetFontRenderer()->LoadFont(fontName);
    
    float size = font->size * Scale();
    float length = pb::Game::Instance()->GetFontRenderer()->MeasureString(fontName, convertedString, size/32.f)*32.f;
    
    return Gwen::Point(length, size);
}

#endif

#endif
