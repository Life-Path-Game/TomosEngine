#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uScreenTexture;

void main() {
    FragColor = texture(uScreenTexture, vTexCoord);
}