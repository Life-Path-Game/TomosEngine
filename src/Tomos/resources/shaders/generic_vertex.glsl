#version 430 core

struct Instance {
    mat4 transform;
};

// Standard vertex attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aTangent; // optional

layout (std430, binding = 0) readonly buffer InstanceBuffer {
    Instance instances[];
};

out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uViewProjection;

void main()
{
    mat4 model = instances[gl_InstanceID].transform;
    gl_Position = uViewProjection * model * vec4(aPosition, 1.0);
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vTexCoord = aTexCoord;
}