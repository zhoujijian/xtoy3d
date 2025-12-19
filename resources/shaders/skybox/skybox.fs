#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube textureCubemap;

void main()
{
    FragColor = texture(textureCubemap, TexCoords);
}