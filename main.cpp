#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <shader.h>
#include <camera.h>
#include <model_node.h>
#include <mesh.h>
#include <mesh_node.h>
#include <light_node.h>
#include <toy3d.h>
#include <learn/tutorial.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void processMouseDragging(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool dragging = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

ModelNode* CreatePlaneModel(Shader& shader, glm::vec3 color) {
    float x1 = -1.0f, x2 = 1.0f;
    float y1 = -1.0f, y2 = 1.0f;
    float z1 = -1.0f, z2 = 1.0f;
    Vertex vertex0;
    Vertex vertex1;
    Vertex vertex2;
    Vertex vertex3;
    vertex0.Position = glm::vec3(x1, 0.0f, z1);
    vertex1.Position = glm::vec3(x2, 0.0f, z1);
    vertex2.Position = glm::vec3(x2, 0.0f, z2);
    vertex3.Position = glm::vec3(x1, 0.0f, z2);
    vector<Vertex> vertices = { vertex0, vertex1, vertex2, vertex3 };
    vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
    Mesh mesh(vertices, indices, {});
    mesh.color = color;
    vector<Mesh> meshes;
    meshes.push_back(mesh);
    ModelNode* plain = new ModelNode(shader, meshes);
    return plain;
}

ModelNode* CreateCubeModel(Shader& shader, glm::vec3 color) {
    float x1 = -1.0f, x2 = 1.0f;
    float y1 = -1.0f, y2 = 1.0f;
    float z1 = -1.0f, z2 = 1.0f;
    Vertex vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7;
    vertex0.Position = glm::vec3(x1, y1, z1);
    vertex1.Position = glm::vec3(x2, y1, z1);
    vertex2.Position = glm::vec3(x2, y2, z1);
    vertex3.Position = glm::vec3(x1, y2, z1);
    vertex4.Position = glm::vec3(x1, y1, z2);
    vertex5.Position = glm::vec3(x2, y1, z2);
    vertex6.Position = glm::vec3(x2, y2, z2);
    vertex7.Position = glm::vec3(x1, y2, z2);
    vector<Vertex> vertices = { vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7 };
    vector<unsigned int> indices = {
        0, 3, 1, 1, 3, 2, // back
        5, 6, 4, 4, 6, 7, // front
        4, 7, 0, 0, 7, 3, // left
        1, 2, 5, 5, 2, 6, // right
        6, 2, 7, 7, 2, 3, // top
        5, 1, 4, 4, 1, 0  // bottom
    };
    Mesh mesh(vertices, indices, {});
    mesh.color = color;
    vector<Mesh> meshes;
    meshes.push_back(mesh);
    ModelNode* plain = new ModelNode(shader, meshes);
    return plain;
}

ModelNode* CreateModelBackpack() {
    Shader ourShader("resources/1.model_loading.vs", "resources/1.model_loading.fs");
    ModelNode* backpack = new ModelNode(ourShader, "resources/objects/backpack/backpack.obj");
    backpack->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    backpack->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    backpack->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    return backpack;
}

void DrawOutline(ToyNode& root) {
    Shader shader("resources/model.vs", "resources/settexture.fs");
    vector<SimpleVertex> vertices = CreateCubeVertices();
    Material material(shader);
    material.textureDiffuse = TextureFromFile("diffuse.png", "resources/objects/box");
    SimpleMeshNode* box = new SimpleMeshNode(vertices, material);
    root.AddChild(box);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    ToyNode root;
    // AddLightsObjects(root);
    AddOutlineObjects(root);

    // ModelNode* backpack = CreateModelBackpack();
    // root.AddChild(backpack);

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        processMouseDragging(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        RenderContext context(&root, projection, view, camera.Position);
        // DrawScene(context);
        DrawOutline(context);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void processMouseDragging(GLFWwindow* window) {
    if (dragging) {
        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float xoffset = static_cast<float>(xpos) - lastX;
        float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void cursor_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    assert(action == GLFW_PRESS || action == GLFW_RELEASE);

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            double xpos;
            double ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);

            dragging = true;
        }
        else {
            dragging = false;
        }
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}