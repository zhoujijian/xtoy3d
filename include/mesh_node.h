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

public:
	SimpleMeshNode(const vector<SimpleVertex>& vertices, const Material& material, const glm::vec3& color);

public:
	void Draw(const RenderContext& context) override;

private:
	unsigned int VAO;
	unsigned int VBO;
	Material material;
	vector<SimpleVertex> vertices;
};

#endif