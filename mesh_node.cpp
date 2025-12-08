#include <mesh_node.h>
#include <glad/glad.h>

SimpleMeshNode::SimpleMeshNode(const vector<SimpleVertex>& vertices, const Material& material, const glm::vec3& color)
	: vertices(vertices), material(material), color(color)
{
	type = ToyNodeType::Mesh;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, TexCoords));

	glBindVertexArray(0);
}

void SimpleMeshNode::Draw(const RenderContext& context) {
	glm::mat4 model = GetTransform();

	glUseProgram(material.shader.ID);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "projection"), 1, GL_FALSE, &context.projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "view"), 1, GL_FALSE, &context.view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material.shader.ID, "model"), 1, GL_FALSE, &model[0][0]);
	
	if (material.textureDiffuse.id == 0 && material.textureSpecular.id == 0) {
		glUniform3fv(glGetUniformLocation(material.shader.ID, "color"), 1, &color[0]);
	}
	else {
		if (material.textureDiffuse.id > 0) {
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(material.shader.ID, "material.texture_diffuse1"), 0);
			glBindTexture(GL_TEXTURE_2D, material.textureDiffuse.id);
		}
		if (material.textureSpecular.id > 0) {
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(material.shader.ID, "material.texture_specular1"), 1);
			glBindTexture(GL_TEXTURE_2D, material.textureSpecular.id);
		}
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	glBindVertexArray(0);
}