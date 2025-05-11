#version 330 core

// Standard vertex attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aTangent; // optional

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

void main()
{
    // Transform normal (using upper 3x3 of the matrix)
    vNormal = mat3(uTransform) * aNormal;
    vTexCoord = aTexCoord;

    // Transform position
    gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
}