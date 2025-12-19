#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform samplerCube textureCubemap;

void main()
{
    vec3 I = normalize(Position - cameraPosition);
    vec3 TexCoords = reflect(I, normalize(Normal));
    FragColor = vec4(texture(textureCubemap, TexCoords).rgb, 1.0);
}