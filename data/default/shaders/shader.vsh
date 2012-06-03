attribute vec4 position;

varying lowp vec4 colorVarying;

uniform vec4 diffuseColor;
uniform mat4 modelViewProjectionMatrix;

void main()
{
    //vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);
    
    colorVarying = diffuseColor;
    
    gl_Position = modelViewProjectionMatrix * position;
}
