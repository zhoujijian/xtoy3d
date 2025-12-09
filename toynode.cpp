#include <toynode.h>

ToyNode::ToyNode() : parent(nullptr), type(ToyNodeType::Empty) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void ToyNode::AddChild(ToyNode* node) {
	children.push_back(node);
}

void ToyNode::Draw(const RenderContext& context) {
	for (auto it = children.begin(); it != children.end(); ++it) {
		(*it)->Draw(context);
	}
}

glm::mat4 ToyNode::GetTransform() const {
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::translate(matrix, position);

	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	matrix = glm::scale(matrix, scale);

	if (parent != nullptr) {
		matrix = parent->GetTransform() * matrix;
	}

	return matrix;
}

vector<ToyNode*> ToyNode::FindNodesByType(ToyNodeType::List type) {
	vector<ToyNode*> result;
	if (this->type == type) {
		result.push_back(this);
	}
	for (auto it = children.begin(); it != children.end(); ++it) {
		vector<ToyNode*> children = (*it)->FindNodesByType(type);
		result.insert(result.end(), children.begin(), children.end());
	}
	return result;
}