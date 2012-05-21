#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

// TODO : Use vertex buffers objects rather than relying on direct OpenGL usage

namespace pb
{

PrimitiveRenderer::PrimitiveRenderer()
{
    
}
    
PrimitiveRenderer::~PrimitiveRenderer()
{
    
}

void PrimitiveRenderer::Update(float time)
{
    _Items.clear();
}

void PrimitiveRenderer::Render(RenderLayer* layer)
{
    ItemList& list = _Items[layer];
    
    if (!list.size())
        return;
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif
    
    for (ItemList::iterator it = list.begin(); it != list.end(); ++it)
    {
        glColor4f(it->color[0], it->color[1], it->color[2], it->color[3]);
        
        switch (it->type)
        {
            case PrimitiveInstance::kTypeEllipse:
            {
                glPushMatrix();
                
                glTranslatef(it->position[0], it->position[1], 0.f);
                glScalef(it->size[0], it->size[1], 1.f);        
                glRotatef(it->rotation[0], 1, 0, 0);
                glRotatef(it->rotation[1], 0, 1, 0);
                glRotatef(it->rotation[2], 0, 0, 1);
                
                GLfloat glVertices[it->segments*2];
                
                float angle = 0;
                for (int i=0; i<it->segments*2; i+=2)
                {
                    angle += M_PI/(it->segments/2);
                    
                    glVertices[i] = cos(angle);
                    glVertices[i+1] = sin(angle);
                }
                
                glVertexPointer(2, GL_FLOAT, 0, glVertices);                
                
                glDrawArrays(GL_LINE_LOOP, 0, it->segments);
                
                glPopMatrix();
                
                break;
            }
            case PrimitiveInstance::kTypeBox:
            {
                glPushMatrix();
                
                glTranslatef(it->position[0], it->position[1], 0);
                glRotatef(it->rotation[0], 1, 0, 0);
                glRotatef(it->rotation[1], 0, 1, 0);
                glRotatef(it->rotation[2], 0, 0, 1);
                
                Vec2 tl(-it->size[0]/2.f, -it->size[1]/2.f);
                Vec2 br(+it->size[0]/2.f, +it->size[1]/2.f);
                
                GLfloat glVertices[8] = { tl[0], br[1], tl[0], tl[1], br[0], tl[1], br[0], br[1] };
                glVertexPointer(2, GL_FLOAT, 0, glVertices);
                
                if (it->solid)
                {
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                }
                else
                    glDrawArrays(GL_LINE_LOOP, 0, 4);
                
                glPopMatrix();
                
                break;
            }
            case PrimitiveInstance::kTypeLine:
            {
                glPushMatrix();
                
                GLfloat glVertices[] = { it->position[0], it->position[1], it->size[0], it->size[1] };
                
                glVertexPointer(2, GL_FLOAT, 0, glVertices);
                
                glDrawArrays(GL_LINES, 0, 2);
                
                glPopMatrix();
                
                break;
            }
        }
    }
    
    glColor4f(1.f, 1.f, 1.f, 1.f);
    
    glDisable(GL_BLEND);
    glDisableClientState(GL_VERTEX_ARRAY);
}
    
void PrimitiveRenderer::AttachEllipse(pb::RenderLayer *layer, Vec2 position, Vec2 size, Vec3 rotation, Vec4 color, int segments)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeEllipse;
    item.position = position;
    item.size = size;
    item.rotation = rotation;
    item.color = color;
    item.segments = segments;
    _Items[layer].push_back(item);
}
    
void PrimitiveRenderer::AttachLine(pb::RenderLayer *layer, Vec2 start, Vec2 end, Vec4 color)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeLine;
    item.position = start;
    item.size = end;
    item.color = color;
    _Items[layer].push_back(item);
}
    
void PrimitiveRenderer::AttachBox(pb::RenderLayer *layer, Vec2 position, Vec2 size, Vec3 rotation, Vec4 color, bool solid)
{
    PrimitiveInstance item;
    item.type = PrimitiveInstance::kTypeBox;
    item.position = position;
    item.size = size;
    item.rotation = rotation;
    item.color = color;
    item.solid = solid;
    _Items[layer].push_back(item);
}
    
}

#endif
