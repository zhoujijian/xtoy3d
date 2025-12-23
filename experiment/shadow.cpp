#include <light_node.h>
#include <learn/tutorial.h>

static void CreatePlaneMesh(ToyNode& root, const Shader& shader) {
    vector<SimpleVertex> vertices = CreatePlainVertices();
    Material material(shader);
    material.shininess = 16.0f;
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    material.textureSpecular = TextureFromFile("specular.png", "resources/objects/box");

	ShadowMeshNode* plane = new ShadowMeshNode(vertices, material);
    plane->SetScale(glm::vec3(32.0f, 1.0f, 32.0f));
    root.AddChild(plane);
}

static void CreateBoxMesh(ToyNode& root, const Shader& shader) {
    vector<SimpleVertex> vertices = CreateCubeVertices();
    Material material(shader);
    material.shininess = 16.0f;
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    material.textureSpecular = TextureFromFile("specular.png", "resources/objects/box");

	ShadowMeshNode* box = new ShadowMeshNode(vertices, material);
    box->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    box->SetPosition(glm::vec3(0.0f, 0.25f, 0.0f));
    root.AddChild(box);
}

static unsigned int framebuffer;
static unsigned int texDepthBuffer;
static unsigned int vaoFramebufferQuad = 0;
static unsigned int vboFramebufferQuad = 0;
static vector<SimpleVertex> verticesFramebufferQuad;
static Shader* shaderFramebufferQuad = nullptr;
static const int SHADOW_WIDTH = 800;
static const int SHADOW_HEIGHT = 600;

static unsigned int AddShadowDepthFramebuffer(int width, int height) {
	glGenTextures(1, &texDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, texDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthBuffer, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	verticesFramebufferQuad = CreateFlatVertices();
	CreateSimpleVertexArray(vaoFramebufferQuad, vboFramebufferQuad, verticesFramebufferQuad);
	shaderFramebufferQuad = new Shader("resources/framebuffer_quad.vs", "resources/shaders/shadow/framebuffer_quad.fs");

	return framebuffer;
}

static void AddDirectionLight(ToyNode& root) {
    ToyLight lightConfDir;
    lightConfDir.type = LightType::Direction;
    lightConfDir.dir.direction = glm::vec3(0.0f, 0.5f, 0.5f);
    lightConfDir.dir.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    lightConfDir.dir.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    lightConfDir.dir.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    LightNode* lightNodeDir = new LightNode(lightConfDir);
    lightNodeDir->color = glm::vec3(1.0f, 0.0f, 0.0f);
    lightNodeDir->SetScale(glm::vec3(0.1f));
    lightNodeDir->SetPosition(glm::vec3(0.0f, 1.0f, 3.0f));

    root.AddChild(lightNodeDir);
}

static void AddPointLight(ToyNode& root) {
    ToyLight lightConfPoint;
    lightConfPoint.type = LightType::Point;
    lightConfPoint.point.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    lightConfPoint.point.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    lightConfPoint.point.specular = glm::vec3(2.0f, 2.0f, 2.0f);
    lightConfPoint.point.constant = 1.0f;
    lightConfPoint.point.linear = 0.22f;
    lightConfPoint.point.quadratic = 0.20f;

    LightNode* lightNodePoint = new LightNode(lightConfPoint);
    lightNodePoint->color = glm::vec3(1.0f, 1.0f, 1.0f);
    lightNodePoint->SetPosition(glm::vec3(1.0f, 2.0f, 1.0f));
    lightNodePoint->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

    root.AddChild(lightNodePoint);
}

static void DrawFramebuffer(const RenderContext& context) {
	// glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	DrawScene(context);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void DrawFramebufferQuad(const RenderContext& context) {
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(shaderFramebufferQuad->ID);
	glUniform1i(glGetUniformLocation(shaderFramebufferQuad->ID, "texture1"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texDepthBuffer);

	glBindVertexArray(vaoFramebufferQuad);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(verticesFramebufferQuad.size()));
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

// #define DEBUG_DRAW_SIMPLE
// #define DEBUG_DRAW_SHADOW_DEPTH

static Shader* shaderSimple = nullptr;
static Shader* shaderShadow = nullptr;
static Shader* shaderShadowDepth = nullptr;
static int drawShadowStep = 0;
static glm::vec3 lightPosition = glm::vec3(2.0f, 3.0f, 4.0f);
// static glm::vec3 lightPosition = glm::vec3(0.0f, 3.0f, 4.0f);

void AddShadowObjects(ToyNode& root) {
	shaderSimple = new Shader("resources/simple.vs", "resources/settexture.fs");
	shaderShadow = new Shader("resources/shaders/shadow/shadow.vs", "resources/shaders/shadow/shadow.fs");
	shaderShadowDepth = new Shader("resources/shaders/shadow/shadow_depth.vs", "resources/shaders/shadow/shadow_depth.fs");	

	Shader* shader = nullptr;
#ifdef DEBUG_DRAW_SIMPLE
	shader = shaderSimple;
#else
	shader = shaderShadowDepth;
#endif
	CreatePlaneMesh(root, *shader);
	CreateBoxMesh(root, *shader);

    AddDirectionLight(root);
	AddShadowDepthFramebuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
}

void DrawShadow(const RenderContext& context) {
#ifdef DEBUG_DRAW_SIMPLE
	DrawScene(context);
#else
	drawShadowStep = 0;
	DrawFramebuffer(context);

	#ifdef DEBUG_DRAW_SHADOW_DEPTH
		DrawFramebufferQuad(context);
	#else
		drawShadowStep = 1;
		DrawScene(context);
	#endif
#endif
}

static glm::mat4 GetLightSpaceMatrix() {
	glm::vec3 to = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(lightPosition, to, up);
	glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 10.0f);
	glm::mat4 matrix = projection * view;
	return matrix;
}

void ShadowMeshNode::DrawSimpleTexture(const RenderContext& context) {
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

void ShadowMeshNode::DrawShadowDepthTexture(const RenderContext& context) {
	shaderShadowDepth->use();

	glm::mat4 model = GetTransform();
	shaderShadowDepth->setMat4("model", model);

	vector<ToyNode*> lightNodes = context.root->FindNodesByType(ToyNodeType::Light);
	vector<ToyNode*>::iterator it_direction_light = std::find_if(
		lightNodes.begin(), lightNodes.end(), [](ToyNode* node) {
		return (static_cast<LightNode*>(node))->light.type == LightType::Direction;
	});
	if (it_direction_light != lightNodes.end()) {
		glm::mat4 matrix = GetLightSpaceMatrix();
		shaderShadowDepth->setMat4("lightSpaceMatrix", matrix);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	glBindVertexArray(0);
}

void ShadowMeshNode::DrawShadowScene(const RenderContext& context) {
	glm::mat4 model = GetTransform();

	shaderShadow->use();
	shaderShadow->setMat4("projection", context.projection);
	shaderShadow->setMat4("view", context.view);
	shaderShadow->setMat4("model", model);
	shaderShadow->setVec3("viewPos", context.viewPosition);

	glm::mat4 matrix = GetLightSpaceMatrix();
	shaderShadow->setMat4("lightSpaceMatrix", matrix);

	shaderShadow->setInt("diffuseTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.textureDiffuse);

	shaderShadow->setInt("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texDepthBuffer);

	vector<ToyNode*> lightNodes = context.root->FindNodesByType(ToyNodeType::Light);
	vector<ToyNode*>::iterator it_direction_light = std::find_if(
		lightNodes.begin(), lightNodes.end(), [](ToyNode* node) {
		return (static_cast<LightNode*>(node))->light.type == LightType::Direction;
	});
	if (it_direction_light != lightNodes.end()) {
		DirectionLight dir = static_cast<LightNode*>(*it_direction_light)->light.dir;
		shaderShadow->setVec3("lightPos", lightPosition);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	glBindVertexArray(0);
}

void ShadowMeshNode::Draw(const RenderContext& context) {
#ifdef DEBUG_DRAW_SIMPLE
	DrawSimpleTexture(context);
#else
	if (drawShadowStep == 0) {
		DrawShadowDepthTexture(context);
	} else {
		DrawShadowScene(context);
	}	
#endif
}