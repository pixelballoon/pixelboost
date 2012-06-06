attribute vec4 position;
attribute vec4 color;
attribute vec2 texture;

varying lowp vec2 textureVarying;
varying lowp vec4 colorVarying;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	colorVarying = color;
    textureVarying = texture;
    
    gl_Position = modelViewProjectionMatrix * position;
}
