#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/render/primitive/primitiveRenderer.h"

namespace libpixel
{

PrimitiveRenderer::PrimitiveRenderer()
{
    
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    
}
    
void PrimitiveRenderer::RenderEllipse(Vec2 position, Vec2 size, Vec3 rotation, Vec4 color, int segments)
{
    glEnable(GL_BLEND);
    
    glPushMatrix();
    
    glTranslatef(position[0], position[1], 0.f);
    glScalef(size[0], size[1], 1.f);        
    glRotatef(rotation[0], 1, 0, 0);
    glRotatef(rotation[1], 0, 1, 0);
    glRotatef(rotation[2], 0, 0, 1);
    
    glColor4f(color[0], color[1], color[2], color[3]);
    
    GLfloat glVertices[segments*2];
    
    float angle = 0;
    for (int i=0; i<segments*2; i+=2)
    {
        angle += M_PI/(segments/2);
        
        glVertices[i] = cos(angle);
        glVertices[i+1] = sin(angle);
    }
        
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDrawArrays(GL_LINE_LOOP, 0, segments);
    
    glPopMatrix();
    
    glDisable(GL_BLEND);
}
    
void PrimitiveRenderer::RenderLine(Vec2 start, Vec2 end, Vec4 color)
{
    glEnable(GL_BLEND);
    
    glPushMatrix();
    
    glColor4f(color[0], color[1], color[2], color[3]);
    
    GLfloat glVertices[] = { start[0], start[1], end[0], end[1] };
    
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDrawArrays(GL_LINES, 0, 2);
    
    glColor4f(1.f, 1.f, 1.f, 1.f);
    
    glPopMatrix();
    
    glDisable(GL_BLEND);
}
    
void PrimitiveRenderer::RenderBox(Vec2 position, Vec2 size, Vec4 color)
{
    glEnable(GL_BLEND);
    
    glColor4f(color[0], color[1], color[2], color[3]);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    Vec2 tl(position[0] - size[0]/2.f, position[1] - size[1]/2.f);
    Vec2 br(position[0] + size[0]/2.f, position[1] + size[1]/2.f);
    
    GLfloat glVertices[8] = { tl[0], br[1], tl[0], tl[1], br[0], tl[1], br[0], br[1] };
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glDisable(GL_BLEND);
}
    
}
