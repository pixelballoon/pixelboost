varying vec2 textureVarying;

uniform vec4 diffuseColor;

uniform sampler2D diffuseTexture;

void main()
{
    gl_FragColor = texture2D(diffuseTexture, textureVarying) * diffuseColor;
}
