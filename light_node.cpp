#include <light_node.h>

LightNode::LightNode(ToyLight light) : light(light), color(glm::vec3(1.0f, 1.0f, 1.0f)) {
    type = ToyNodeType::Light;

    float x1 = -0.5f, x2 = 0.5f;
    float y1 = -0.5f, y2 = 0.5f;
    float z1 = -0.5f, z2 = 0.5f;
    vector<glm::vec3> vertices = {
        glm::vec3(x1, y1, z1),
        glm::vec3(x2, y1, z1),
        glm::vec3(x2, y2, z1),
        glm::vec3(x1, y2, z1),
        glm::vec3(x1, y1, z2),
        glm::vec3(x2, y1, z2),
        glm::vec3(x2, y2, z2),
        glm::vec3(x1, y2, z2)
    };

    indices = {
        0, 3, 1, 1, 3, 2, // back
        5, 6, 4, 4, 6, 7, // front
        4, 7, 0, 0, 7, 3, // left
        1, 2, 5, 5, 2, 6, // right
        6, 2, 7, 7, 2, 3, // top
        5, 1, 4, 4, 1, 0  // bottom
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);

    shader = new Shader("resources/simple.vs", "resources/simple.fs");
}

LightNode::~LightNode() {
    if (shader != NULL) {
        delete shader;
        shader = NULL;
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}

void LightNode::Draw(const RenderContext& context) {
    glUseProgram(shader->ID);
    glm::mat4 model = GetTransform();
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, &context.projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, &context.view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "color"), 1, &color[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}