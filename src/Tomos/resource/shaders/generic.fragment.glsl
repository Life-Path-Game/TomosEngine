#version 330 core
layout (location = 0) out vec4 color;

in vec3 vertexPos;
in vec4 vertexColor;

void main()
{
    color = vertexColor;
}