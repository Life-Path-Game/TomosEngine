#version 330 core

// Material properties as uniforms instead of UBO
uniform vec4 uBaseFactor;
uniform vec3 uEmissionFactor;
uniform float uMetalFactor;
uniform float uRoughFactor;
uniform float uNormalScale;
uniform float uAlphaCutoff;

uniform sampler2D uBaseTexture;
uniform sampler2D uMetalRoughTexture;
uniform sampler2D uNormalTexture;

in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColor;

void main()
{
    vec4 baseColor = texture(uBaseTexture, vTexCoord) * uBaseFactor;
    vec3 normal = normalize(vNormal);

    // Simple directional light
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * baseColor.rgb;

    // Combine with emission
    vec3 result = diffuse + uEmissionFactor;

    FragColor = baseColor;
}