#version 150 core

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D texGrass;
uniform sampler2D texDirt;

uniform float time;

void main()
{
    vec4 colGrass = texture(texGrass, Texcoord);
    vec4 colDirt = texture(texDirt, Texcoord);

    //outColor = mix(colGrass, colDirt, time);
    outColor = texture(texDirt,
            vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)
            ) * vec4(0.7, 0.7, 1.0, 1.0);

}
