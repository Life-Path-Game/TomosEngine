#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec3 vertexPos;
out vec4 vertexColor;

void main()
{
    vertexPos = aPos;
    vertexColor = aColor;
    gl_Position = viewProjection * transform  * vec4(aPos, 1.0);
}