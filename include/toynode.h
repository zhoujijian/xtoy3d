#ifndef _TOY_NODE_H_
#define _TOY_NODE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <toy3d.h>

using namespace std;

class ToyNode;

struct ToyNodeType {
	enum List {
		Empty = 0,
		Light,
		Model,
		Mesh,
		Camera
	};
};

struct RenderContext {
	ToyNode* root;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 viewPosition;

	RenderContext(
		ToyNode* root,
		const glm::mat4& projection,
		const glm::mat4& view,
		const glm::vec3& viewPosition)
		: root(root), projection(projection), view(view), viewPosition(viewPosition) {}
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

	vector<ToyNode*> FindNodesByType(ToyNodeType::List type);

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