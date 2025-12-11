#ifndef _TOY_TUTORIAL_H_
#define _TOY_TUTORIAL_H_

#include <mesh_node.h>

void DrawScene(const RenderContext& context);
void AddLightsObjects(ToyNode& root);
void AddOutlineObjects(ToyNode& root);
void DrawOutline(const RenderContext& context);

class TextureMeshNode : public SimpleMeshNode {
public:
	TextureMeshNode(
		const vector<SimpleVertex>& vertices,
		const Material& material) : SimpleMeshNode(vertices, material) { }

public:
	void Draw(const RenderContext& context) override;
};

class OutlineMeshNode : public SimpleMeshNode {
public:
	bool outline = false;

public:
	OutlineMeshNode(
		const vector<SimpleVertex>& vertices,
		const Material& material) : SimpleMeshNode(vertices, material) { }

public:
	void Draw(const RenderContext& context) override;
};

#endif
