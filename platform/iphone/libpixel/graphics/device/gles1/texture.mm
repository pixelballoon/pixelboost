#include "libpixel/graphics/device/texture.h"

using namespace libpixel;

GLuint libpixel::LoadTexture(void* image, bool createMips)
{
    UIImage* uiImage = (UIImage*)image;
    
    CGImage* cgImage = [uiImage CGImage];
	const int width = CGImageGetWidth(cgImage);
	const int height = CGImageGetHeight(cgImage);
	
	const int dataSize = width * height * 4;
	uint8_t* textureData = (uint8_t*)calloc(1, dataSize);
	CGContext* textureContext = CGBitmapContextCreate(textureData, width, height, 8, width * 4, CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
	CGContextRelease(textureContext);
	
	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
    
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
	
	glBindTexture(GL_TEXTURE_2D, 0);
	free(textureData);
    
    return handle;
}

GLuint libpixel::LoadTexture(const std::string& path, bool createMips)
{
	UIImage* image = [[UIImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];
    
    if (!image)
        return 0;
    
    GLuint handle = LoadTexture(image, createMips);
	
	[image release];
	
	return handle;
}

void libpixel::TextureGLES1::Load(const std::string& image, bool createMips)
{
    _Texture = LoadTexture(image);
}