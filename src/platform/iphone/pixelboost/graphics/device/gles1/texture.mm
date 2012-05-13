#ifdef PIXELBOOST_PLATFORM_IOS

#include <string>

#include "pixelboost/graphics/device/texture.h"

using namespace pb;

void pb::TextureGLES1::Load(const unsigned char* data, bool createMips, TextureFormat format)
{
    
}

void pb::TextureGLES1::Load(const std::string& path, bool createMips)
{
    UIImage* image = [[UIImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];
    
    if (!image)
        return;
    
    CGImage* cgImage = [image CGImage];
	const int width = CGImageGetWidth(cgImage);
	const int height = CGImageGetHeight(cgImage);
	
	const int dataSize = width * height * 4;
	uint8_t* textureData = (uint8_t*)calloc(1, dataSize);
	CGContext* textureContext = CGBitmapContextCreate(textureData, width, height, 8, width * 4, CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
	CGContextRelease(textureContext);
	
	glGenTextures(1, &_Texture);

    Texture* previousTexture = _Device->BindTexture(this);
    
    if (createMips)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	
    _Device->BindTexture(previousTexture);
    
	free(textureData);
	
	[image release];
}

#endif
