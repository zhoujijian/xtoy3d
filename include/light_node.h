#ifndef _TOY_LIGHT_NODE_H_
#define _TOY_LIGHT_NODE_H_

#include <toynode.h>

struct DirectionLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

struct LightType {
	enum List {
		Direction,
		Point,
		Spot
	};
};;

struct ToyLight {
	LightType::List type;
	union {
		DirectionLight dir;
		PointLight point;
		SpotLight spot;
	};
};

class LightNode : public ToyNode {
public:
	glm::vec3 color;
	ToyLight light;

public:
	LightNode(ToyLight light);
	~LightNode();

	void Draw(const RenderContext& context) override;

private:	
	Shader* shader;
	vector<unsigned int> indices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

#endif