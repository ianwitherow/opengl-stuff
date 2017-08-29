#version 150 core

in vec3 position;
in vec2 texcoord;

out vec2 Texcoord;

uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
    Texcoord = texcoord;

    gl_Position = proj * view * model * vec4(position, 1.0);
}
