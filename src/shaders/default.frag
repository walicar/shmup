#version 410 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D ourTexture;
uniform float transparency;

void main()
{
    vec4 texColor = texture(ourTexture, texCoord);
    FragColor = vec4(texColor.rgb, texColor.a * transparency);
}
