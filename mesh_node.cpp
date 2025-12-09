#include <mesh_node.h>
#include <glad/glad.h>
#include <light_node.h>

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
	material.shader.setVec3("viewPos", context.viewPosition);
	material.shader.setFloat("material.shininess", material.shininess);
	
	if (material.textureDiffuse == 0 && material.textureSpecular == 0) {
		glUniform3fv(glGetUniformLocation(material.shader.ID, "color"), 1, &color[0]);
	}
	else {
		if (material.textureDiffuse > 0) {
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(material.shader.ID, "material.texture_diffuse1"), 0);
			glBindTexture(GL_TEXTURE_2D, material.textureDiffuse);
		}
		if (material.textureSpecular > 0) {
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(material.shader.ID, "material.texture_specular1"), 1);
			glBindTexture(GL_TEXTURE_2D, material.textureSpecular);
		}
	}

	vector<ToyNode*> lightNodes = context.root->FindNodesByType(ToyNodeType::Light);

	vector<ToyNode*>::iterator it_direction_light = std::find_if(
		lightNodes.begin(), lightNodes.end(), [](ToyNode * node) {
			return (static_cast<LightNode*>(node))->light.type == LightType::Direction;
		});
	if (it_direction_light != lightNodes.end()) {
		DirectionLight dir = static_cast<LightNode*>(*it_direction_light)->light.dir;
		material.shader.setVec3("dirLight.direction", dir.direction);
		material.shader.setVec3("dirLight.ambient", dir.ambient);
		material.shader.setVec3("dirLight.diffuse", dir.diffuse);
		material.shader.setVec3("dirLight.specular", dir.specular);
	}

	vector<ToyNode*>::iterator it_point_light = std::find_if(
		lightNodes.begin(), lightNodes.end(), [](ToyNode* node) {
			return (static_cast<LightNode*>(node))->light.type == LightType::Point;
		});
	if (it_point_light != lightNodes.end()) {
		LightNode* node = static_cast<LightNode*>(*it_point_light);
		PointLight point = node->light.point;
		material.shader.setVec3("pointLights[0].position", node->GetPosition());
		material.shader.setVec3("pointLights[0].ambient", point.ambient);
		material.shader.setVec3("pointLights[0].diffuse", point.diffuse);
		material.shader.setVec3("pointLights[0].specular", point.specular);
		material.shader.setFloat("pointLights[0].constant", point.constant);
		material.shader.setFloat("pointLights[0].linear", point.linear);
		material.shader.setFloat("pointLights[0].quadratic", point.quadratic);
	}

	vector<ToyNode*>::iterator it_spot_light = std::find_if(
		lightNodes.begin(), lightNodes.end(), [](ToyNode* node) {
			return (static_cast<LightNode*>(node))->light.type == LightType::Spot;
		});
	if (it_spot_light != lightNodes.end()) {
		LightNode* node = static_cast<LightNode*>(*it_spot_light);
		SpotLight spot = node->light.spot;
		material.shader.setVec3("spotLights[0].position", node->GetPosition());
		material.shader.setVec3("spotLights[0].direction", spot.direction);
		material.shader.setVec3("spotLights[0].ambient", spot.ambient);
		material.shader.setVec3("spotLights[0].diffuse", spot.diffuse);
		material.shader.setVec3("spotLights[0].specular", spot.specular);
		material.shader.setFloat("spotLights[0].constant", spot.constant);
		material.shader.setFloat("spotLights[0].linear", spot.linear);
		material.shader.setFloat("spotLights[0].quadratic", spot.quadratic);
		material.shader.setFloat("spotLights[0].cutOff", spot.cutOff);
		material.shader.setFloat("spotLights[0].outerCutOff", spot.outerCutOff);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	glBindVertexArray(0);
}