#include <learn/tutorial.h>

void TextureMeshNode::Draw(const RenderContext& context) {
	glm::mat4 model = GetTransform();

	glUseProgram(material.shader.ID);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "projection"), 1, GL_FALSE, &context.projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "view"), 1, GL_FALSE, &context.view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "model"), 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(material.shader.ID, "texture1"), 0);
	glBindTexture(GL_TEXTURE_2D, material.textureDiffuse);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	glBindVertexArray(0);
}

void DrawScene(const RenderContext& context) {
    vector<ToyNode*> children = context.root->GetChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->Draw(context);
    }
}

void AddBoxMesh(ToyNode& root) {
	vector<SimpleVertex> vertices = CreateCubeVertices();
	Shader shader("resources/model.vs", "resources/settexture.fs");
	Material material(shader);
	material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
	TextureMeshNode* box = new TextureMeshNode(vertices, material);
	box->SetPosition(glm::vec3(0.0f, 1.0f, -0.5f));
	root.AddChild(box);
}