#include <learn/tutorial.h>
#include <GLFW/glfw3.h>

static unsigned int houseVAO = 0;
static unsigned int houseVBO = 0;
static Shader* shaderHouse = NULL;

static unsigned int boxVAO = 0;
static unsigned int boxVBO = 0;
static unsigned int boxTexture = 0;
static Shader* shaderBox = NULL;
static vector<SimpleVertex> boxVertices;

void AddBox(const char* pathVS, const char* pathFS, const char* pathGS) {
    boxVertices = CreateCubeVertices();
    boxTexture = TextureFromFile("diffuse.png", "resources/objects/box");
    shaderBox = new Shader(pathVS, pathFS, pathGS);

    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(SimpleVertex), &boxVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, TexCoords));
    glBindVertexArray(0);
}

void AddExplode() {
    AddBox("resources/geometry_explode.vs", "resources/settexture.fs", "resources/geometry_explode.gs");
}

void DrawExplode(const RenderContext& context) {
    glUseProgram(shaderBox->ID);

    glm::mat4 model = glm::mat4(1.0f);    
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderBox->ID, "projection"), 1, GL_FALSE, &context.projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderBox->ID, "view"), 1, GL_FALSE, &context.view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderBox->ID, "model"), 1, GL_FALSE, &model[0][0]);

    float time = static_cast<float>(glfwGetTime());
    glUniform1f(glGetUniformLocation(shaderBox->ID, "time"), time);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderBox->ID, "texture1"), 0);
	glBindTexture(GL_TEXTURE_2D, boxTexture);

	glBindVertexArray(boxVAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(boxVertices.size()));
	glBindVertexArray(0);
}

void AddHouse() {
    // positions(vec2) | color(vec3)
    float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

    glGenVertexArrays(1, &houseVAO);
    glGenBuffers(1, &houseVBO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    shaderHouse = new Shader("resources/geometry.vs", "resources/geometry.fs", "resources/geometry.gs");
}

void DrawHouse(const RenderContext& context) {
    glUseProgram(shaderHouse->ID);

    glBindVertexArray(houseVAO);
    glDrawArrays(GL_POINTS, 0, 4);
    glBindVertexArray(0);
}

void AddGeometryObjects(ToyNode& root) {
    // AddHouse();
    AddExplode();
}

void DrawGeometry(const RenderContext& context) {
    // DrawHouse(context);
    DrawExplode(context);
}