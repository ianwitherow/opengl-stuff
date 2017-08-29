#version 150 core

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D theTexture;

void main()
{
    outColor = texture(theTexture, Texcoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
