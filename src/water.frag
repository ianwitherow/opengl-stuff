#version 150 core

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D waterTexture;

uniform float time;

void main()
{
    outColor = texture(waterTexture,
                       vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)
                       ) * vec4(0.7, 0.7, 1.0, 1.0);
}
