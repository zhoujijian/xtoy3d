#include <learn/tutorial.h>
#include <light_node.h>

static SimpleMeshNode* CreatePlaneMesh(const Shader& shader) {
    vector<SimpleVertex> vertices = CreatePlainVertices();
    Material material(shader);
    material.shininess = 16.0f;
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    material.textureSpecular = TextureFromFile("specular.png", "resources/objects/box");

    SimpleMeshNode* plain = new SimpleMeshNode(vertices, material);
    return plain;
}

static SimpleMeshNode* CreateBoxMesh(const Shader& shader) {
    vector<SimpleVertex> vertices = CreateCubeVertices();
    Material material(shader);
    material.shininess = 16.0f;
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    material.textureSpecular = TextureFromFile("specular.png", "resources/objects/box");
    SimpleMeshNode* cube = new SimpleMeshNode(vertices, material);
    return cube;
}

static void AddDirectionLight(ToyNode& root) {
    ToyLight lightConfDir;
    lightConfDir.type = LightType::Direction;
    lightConfDir.dir.direction = glm::vec3(0.0f, 0.2f, 0.5f);
    lightConfDir.dir.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    lightConfDir.dir.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    lightConfDir.dir.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    LightNode* lightNodeDir = new LightNode(lightConfDir);
    lightNodeDir->color = glm::vec3(1.0f, 0.0f, 0.0f);
    lightNodeDir->SetScale(glm::vec3(0.1f));
    lightNodeDir->SetPosition(lightConfDir.dir.direction * glm::vec3(1.0));

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

static void AddSpotLight(ToyNode& root) {
    ToyLight lightConfSpot;
    lightConfSpot.type = LightType::Spot;
    lightConfSpot.spot.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightConfSpot.spot.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    lightConfSpot.spot.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    lightConfSpot.spot.specular = glm::vec3(2.0f, 2.0f, 2.0f);
    lightConfSpot.spot.constant = 1.0f;
    lightConfSpot.spot.linear = 0.14f;
    lightConfSpot.spot.quadratic = 0.07f;
    lightConfSpot.spot.cutOff = glm::cos(glm::radians(12.5f));
    lightConfSpot.spot.outerCutOff = glm::cos(glm::radians(25.0f));

    LightNode* lightNodeSpot = new LightNode(lightConfSpot);
    lightNodeSpot->color = glm::vec3(1.0f, 0.0f, 1.0f);
    lightNodeSpot->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    lightNodeSpot->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

    root.AddChild(lightNodeSpot);
}

void AddLightsObjects(ToyNode& root) {
    Shader shader("resources/model.vs", "resources/model.fs");

    SimpleMeshNode* plane = CreatePlaneMesh(shader);
    plane->SetScale(glm::vec3(8.0f, 1.0f, 8.0f));
    root.AddChild(plane);

    SimpleMeshNode* box = CreateBoxMesh(shader);
    box->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    box->SetPosition(glm::vec3(0.0f, 1.5f, -0.5f));
    root.AddChild(box);

    AddDirectionLight(root);
    AddPointLight(root);
    AddSpotLight(root);
}