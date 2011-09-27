#ifdef LIBPIXEL__PLATFORM__MACOSX

#include "libpixel/render/textureLoader.h"

namespace libpixel
{

GLuint LoadTexture(const std::string& path)
{
	NSImage* image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];
    
    if (!image)
        return 0;
	
	CGImage* cgImage = [image CGImageForProposedRect:nil context:nil hints:nil];
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
	
	glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	free(textureData);
    [image release];
	
	return handle;
}

}

#endif LIBPIXEL__PLATFORM__MACOSX