#ifndef _TOY_MESH_NODE_H_
#define _TOY_MESH_NODE_H_

#include <vector>
#include <toynode.h>
#include <toy3d.h>
#include <glm/glm.hpp>

using namespace std;

class SimpleMeshNode : public ToyNode {
public:
	glm::vec3 color;
	Material material;

public:
	SimpleMeshNode(
		const vector<SimpleVertex>& vertices,
		const Material& material,
		const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));

public:
	void Draw(const RenderContext& context) override;

protected:
	unsigned int VAO;
	unsigned int VBO;
	vector<SimpleVertex> vertices;
};

#endif