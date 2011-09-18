#include "box2dDebugRender.h"

#include <OpenGLES/ES1/gl.h>

/// Draw a closed polygon provided in CCW order.
void PhysicsRender::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1.f);
	
	GLfloat* glVertices = new GLfloat[vertexCount*2];
	
	for (int i=0; i<vertexCount; i++)
	{
		glVertices[(i*2)] = vertices[i].x;
		glVertices[(i*2)+1] = vertices[i].y;
	}
	
	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
	
	delete[] glVertices;
}

/// Draw a solid closed polygon provided in CCW order.
void PhysicsRender::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 0.5f);
	  
	GLfloat* glVertices = new GLfloat[vertexCount*2];
	
	for (int i=0; i<vertexCount; i++)
	{
		glVertices[(i*2)] = vertices[i].x;
		glVertices[(i*2)+1] = vertices[i].y;
	}
	
	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	
	delete[] glVertices;
}

/// Draw a circle.
void PhysicsRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int segments = 20;
	
	glColor4f(color.r, color.g, color.b, 1.f);
	
	GLfloat glVertices[segments*2];
	
	for (int i=0; i<segments; i++)
	{
		glVertices[(i*2)] = center.x + radius*cos(i*(6.2831853f/segments));
		glVertices[(i*2)+1] = center.y + radius*sin(i*(6.2831853f/segments));
	}
	
	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_LINE_LOOP, 0, segments);
}

/// Draw a solid circle.
void PhysicsRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1.f);
	
	{
		const int segments = 20;

		GLfloat glVertices[segments*2];
	
		for (int i=0; i<segments; i++)
		{
			glVertices[(i*2)] = center.x + radius*cos(i*(6.2831853f/segments));
			glVertices[(i*2)+1] = center.y + radius*sin(i*(6.2831853f/segments));
		}
		
		glVertexPointer(2, GL_FLOAT, 0, glVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, segments);
	}
	
	{
		glColor4f(0.f, 0.f, 0.f, 1.f);
		
		GLfloat glVertices[4];
		
		glVertices[0] = center.x;
		glVertices[1] = center.y;
		glVertices[2] = center.x + axis.x;
		glVertices[3] = center.y + axis.y;
		
		glVertexPointer(2, GL_FLOAT, 0, glVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glDrawArrays(GL_LINES, 0, 2);
	}
}

/// Draw a line segment.
void PhysicsRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1.f);
	
	GLfloat glVertices[4];
	
	glVertices[0] = p1.x;
	glVertices[1] = p1.y;
	glVertices[2] = p2.x;
	glVertices[3] = p2.y;

	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_LINES, 0, 2);
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void PhysicsRender::DrawTransform(const b2Transform& xf)
{
	glColor4f(.5f, .5f, .5f, 1.f);
	
	GLfloat glVertices[4];
	
	glVertices[0] = xf.position.x;
	glVertices[1] = xf.position.y;
	glVertices[2] = xf.position.x + 2.f*cos(xf.GetAngle());
	glVertices[3] = xf.position.y + 2.f*sin(xf.GetAngle());
	
	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glDrawArrays(GL_LINES, 0, 2);
}