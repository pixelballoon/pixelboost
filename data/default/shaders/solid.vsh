attribute vec4 position;

varying lowp vec4 colorVarying;

uniform vec4 diffuseColor;
uniform mat4 modelViewProjectionMatrix;

void main()
{
    colorVarying = diffuseColor;
    
    gl_Position = modelViewProjectionMatrix * position;
}
