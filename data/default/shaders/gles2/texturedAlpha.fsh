varying lowp vec2 textureVarying;

varying lowp vec4 colorVarying;

uniform sampler2D diffuseTexture;

void main()
{
    gl_FragColor = texture2D(diffuseTexture, textureVarying) * colorVarying;
}
