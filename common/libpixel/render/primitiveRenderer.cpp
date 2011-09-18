#include "libpixel/os/definitions.h"
#include "libpixel/render/primitiveRenderer.h"
#include "libpixel/render/gl.h"

namespace libpixel
{

PrimitiveRenderer::PrimitiveRenderer()
{
    
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    
}
    
void PrimitiveRenderer::RenderEllipse(Vec2 position, Vec2 size, Vec3 rotation, Vec3 colour)
{
    glPushMatrix();
    
    glTranslatef(position[0], position[1], 0.f);
    glScalef(size[0], size[1], 1.f);        
    glRotatef(rotation[0], 1, 0, 0);
    glRotatef(rotation[1], 0, 1, 0);
    glRotatef(rotation[2], 0, 0, 1);
    glColor4f(colour[0], colour[1], colour[2], 1.f);
    
    GLfloat glVertices[64];
    
    float angle = 0;
    for (int i=0; i<64; i+=2)
    {
        angle += M_PI/16;
        
        glVertices[i] = cos(angle);
        glVertices[i+1] = sin(angle);
    }
        
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDrawArrays(GL_LINE_LOOP, 0, 32);
    
    glPopMatrix();
}
    
void PrimitiveRenderer::RenderLine(Vec2 start, Vec2 end, Vec3 colour)
{
    glPushMatrix();
    
    glColor4f(colour[0], colour[1], colour[2], 1.f);
    
    GLfloat glVertices[] = { start[0], start[1], end[0], end[1] };
    
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDrawArrays(GL_LINES, 0, 2);
    
    glPopMatrix();
}
    
}
