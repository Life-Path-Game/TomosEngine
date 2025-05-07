#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aTangent; // optional

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uTransform;
uniform mat4 uViewProjection;

void main()
{
    vNormal = mat3(uTransform) * aNormal; // Transform normal to world space
    vTexCoord = aTexCoord; // Pass texture coordinates to fragment shader

    gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0); // Transform position
}
