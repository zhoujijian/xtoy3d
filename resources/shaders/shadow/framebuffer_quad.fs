#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    float depth = texture(texture1, TexCoords).r;
    FragColor = vec4(depth, depth, depth, 1.0);
}