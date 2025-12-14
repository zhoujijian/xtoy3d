#include <learn/tutorial.h>

static OutlineMeshNode* box = NULL;
static TextureMeshNode* plane = NULL;
static Shader* shader = NULL;
static Shader* shaderOutline = NULL;

OutlineMeshNode* CreateBoxMesh() {
    Material material(*shader);
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    vector<SimpleVertex> vertices = CreateCubeVertices();
    return new OutlineMeshNode(vertices, material);
}

TextureMeshNode* CreatePlaneMesh() {
    vector<SimpleVertex> vertices = CreatePlainVertices();
    Shader shader("resources/model.vs", "resources/settexture.fs");
    Material material(shader);
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    return new TextureMeshNode(vertices, material);
}

void AddOutlineObjects(ToyNode& root) {
    shader = new Shader("resources/model.vs", "resources/settexture.fs");
    shaderOutline = new Shader("resources/model.vs", "resources/setcolor.fs");

    box = CreateBoxMesh();
    box->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    box->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    root.AddChild(box);

    plane = CreatePlaneMesh();
    plane->SetScale(glm::vec3(3.0f, 1.0f, 3.0f));
    root.AddChild(plane);

    glEnable(GL_STENCIL_TEST);

    // @param1: [GL_KEEP: if stencil test fail, keep stencil_buffer_value]
    // @param2: [GL_KEEP: if stencil test pass, depth test fail, keep stencil_buffer_value]
    // @param3: [GL_REPLACE: if stencil test pass, depth test pass,
    //   replace stencil_buffer_value with [stencil_ref & mask(set by glStencilMask)]]
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void DrawOutline(const RenderContext& context) {
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0x00);
    plane->Draw(context);

    // [1(stencil_ref) & 0xFF(func_mask)] COMPARE [stencil_buffer_value & 0xFF(func_mask)]
    // GL_ALWAYS: fragment always pass(stencil test && depth test) => GL_KEEP(keep stencil_buffer_value) according to [glStencilOp]
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    // new_stencil_value = [old_stencil_value & ~0xFF(mask)] | [new_value & 0xFF(mask)]
    // write 8 bits to stencil_buffer_value: [1(stencil_ref) & 0xFF(mask)]
    glStencilMask(0xFF);

    box->material.shader = *shader;
    box->outline = false;
    box->SetPosition(glm::vec3(0.0f, 0.6f, 0.0f));
    box->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    box->Draw(context);

    // GL_NOTEQUAL: fragment pass when [1(stencil_ref) & 0xFF(func_mask)] != [stencil_buffer_value & 0xFF(func_mask)]
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // new_stencil_value = [old_stencil_value & ~0x00(mask)] | [new_value & 0x00(mask)]
    // do nothing: write 0 bits to stencil_buffer_value
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    box->material.shader = *shaderOutline;
    box->outline = true;
    box->color = glm::vec3(1.0f, 0.0f, 0.0f);
    // box->SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
    box->SetScale(glm::vec3(0.55f, 0.55f, 0.55f));
    box->Draw(context);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void OutlineMeshNode::Draw(const RenderContext& context) {
    glm::mat4 model = GetTransform();

    glUseProgram(material.shader.ID);
    glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "projection"), 1, GL_FALSE, &context.projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "view"), 1, GL_FALSE, &context.view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "model"), 1, GL_FALSE, &model[0][0]);

    if (outline) {
        // setcolor.fs
        material.shader.setVec3("color", color);
    }
    else {
        // settexture.fs
        glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(material.shader.ID, "texture1"), 0);
		glBindTexture(GL_TEXTURE_2D, material.textureDiffuse);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
    glBindVertexArray(0);
}