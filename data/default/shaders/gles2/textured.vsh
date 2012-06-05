attribute vec4 position;
attribute vec2 texture;

varying lowp vec2 textureVarying;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    textureVarying = texture;
    
    gl_Position = modelViewProjectionMatrix * position;
}
