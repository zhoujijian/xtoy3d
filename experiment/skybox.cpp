#include <learn/tutorial.h>

static unsigned int skyboxVBO = 0;
static unsigned int skyboxVAO = 0;
static unsigned int skyboxCubemapTexture = 0;
static Shader* skyboxShader = NULL;

static unsigned int reflectVAO = 0;
static unsigned int reflectVBO = 0;
vector<SimpleVertex> reflectVertices;
static Shader* reflectRefractShader = NULL;

void AddBox(ToyNode& root, const char* path_vs, const char* path_fs) {
    reflectVertices = CreateCubeVertices();
    glGenVertexArrays(1, &reflectVAO);
    glGenBuffers(1, &reflectVBO);

    glBindVertexArray(reflectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, reflectVBO);
    glBufferData(GL_ARRAY_BUFFER, reflectVertices.size() * sizeof(SimpleVertex), &reflectVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, Normal));

    glBindVertexArray(0);

    reflectRefractShader = new Shader(path_vs, path_fs);
}

void AddReflectBox(ToyNode& root) {
    AddBox(root, "resources/skybox_reflect.vs", "resources/skybox_reflect.fs");
}

void AddRefractBox(ToyNode& root) {
    AddBox(root, "resources/skybox_reflect.vs", "resources/skybox_refract.fs");
}

static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int LoadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void AddSkyboxObjects(ToyNode& root) {
    // some of skybox cubemap textures not vertically flipped
    stbi_set_flip_vertically_on_load(false);
    vector<std::string> faces {
        "resources/skybox/right.jpg",
        "resources/skybox/left.jpg",
        "resources/skybox/top.jpg",
        "resources/skybox/bottom.jpg",
        "resources/skybox/front.jpg",
        "resources/skybox/back.jpg"
    };
    skyboxCubemapTexture = LoadCubemap(faces);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    skyboxShader = new Shader("resources/skybox.vs", "resources/skybox.fs");

    // AddReflectBox(root);
    AddRefractBox(root);
}

void DrawSkybox(const RenderContext& context) {
    reflectRefractShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemapTexture);
    glm::mat4 model = glm::mat4(1.0f);
    reflectRefractShader->setMat4("model", model);
    reflectRefractShader->setMat4("view", context.view);
    reflectRefractShader->setMat4("projection", context.projection);
    reflectRefractShader->setVec3("cameraPosition", context.viewPosition);

    glBindVertexArray(reflectVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(reflectVertices.size()));
    glBindVertexArray(0);

    // change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    skyboxShader->use();

    glm::mat4 view = glm::mat4(glm::mat3(context.view)); // remove translation from the view matrix
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", context.projection);
    skyboxShader->setInt("textureCubemap", 0);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(skyboxVertices) / (sizeof(float) * 3));
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}