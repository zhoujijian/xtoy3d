#ifndef _TOY_NODE_H_
#define _TOY_NODE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <toy3d.h>

using namespace std;

struct ToyNodeType {
	enum List {
		Empty = 0,
		Light,
		Model,
		Mesh,
		Camera
	};
};

class ToyNode {
public:
	ToyNode();

public:	
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }
	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetRotation(glm::vec3 rotation) { this->rotation = rotation; }
	void SetScale(glm::vec3 scale) { this->scale = scale; }
	glm::mat4 GetTransform() const;
	vector<ToyNode*> GetChildren() const { return children; }
	ToyNodeType::List GetNodeType() const { return type; }

public:
	void AddChild(ToyNode* node);
	virtual void Draw(const RenderContext& context);

protected:
	ToyNodeType::List type;
	ToyNode* parent;
	vector<ToyNode*> children;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

#endif