#ifndef _TOY3D_H_
#define _TOY3D_H_

#include <vector>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>

#include <shader.h>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct SimpleVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    Texture() { id = 0; }

    unsigned int id;
    string type;
    string path;
};

struct Material {
    Material(const Shader& shader) : shader(shader), shininess(0) { }

    Shader shader;
    float shininess;
    Texture textureDiffuse;
    Texture textureSpecular;
};

struct RenderContext {
    glm::mat4 projection;
    glm::mat4 view;

    RenderContext(const glm::mat4& projection, const glm::mat4& view)
        : projection(projection), view(view) {}
};

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
vector<SimpleVertex> CreateCubeVertices();

#endif