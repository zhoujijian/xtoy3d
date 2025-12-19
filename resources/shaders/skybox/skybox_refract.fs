#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform samplerCube textureCubemap;

// https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/06%20Cubemaps/
void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPosition);
    vec3 TexCoords = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(textureCubemap, TexCoords).rgb, 1.0);
}