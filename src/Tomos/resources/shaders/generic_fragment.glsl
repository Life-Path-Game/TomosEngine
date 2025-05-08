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
    // Sample textures
    vec4 baseColor = texture(uBaseTexture, vTexCoord);
    vec4 metalRoughColor = texture(uMetalRoughTexture, vTexCoord);
    vec3 normalColor = texture(uNormalTexture, vTexCoord).xyz * 2.0 - 1.0;

    // Apply material properties
    vec3 normal = normalize(vNormal + normalColor * uNormalScale);
    float metallic = metalRoughColor.r * uMetalFactor;
    float roughness = metalRoughColor.g * uRoughFactor;

    // Calculate final color
    FragColor = baseColor * uBaseFactor;
    FragColor.rgb += uEmissionFactor;

    // Alpha cutoff
    if (FragColor.a < uAlphaCutoff)
        discard;
}