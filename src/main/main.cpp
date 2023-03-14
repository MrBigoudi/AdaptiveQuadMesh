#include <cstdlib>

#include "mesh.hpp"
#include "scene.hpp"
#include "opengl.hpp"
#include "utils.hpp"
#include "maths.hpp"

// camera
scene::Camera camera(maths::Vector3(0.0f, 0.0f, 100.0f));

// transformation matrix
glm::mat4 trans = glm::mat4(1.0f);

float rotationAngle = glm::radians(10.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback([[maybe_unused]] GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovements(xoffset, yoffset);
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        trans = glm::rotate(trans, -rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        trans = glm::rotate(trans, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        trans = glm::rotate(trans, -rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        trans = glm::rotate(trans, rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){

    opengl::initGLFW();
    GLFWwindow* window = opengl::createWindow(SCR_WIDTH, SCR_HEIGHT, "test", GLFW_TRUE);
    opengl::initGLAD();
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    opengl::Shader shader = opengl::Shader("src/shaders/vert.glsl", "src/shaders/frag.glsl");

    glEnable(GL_DEPTH_TEST);

    // scene::Object object = scene::Object("media/objects/venus.obj");
    scene::Object object = scene::Object("media/objects/chess_piece.obj");

    const float* vertices = object.getVertices();
    const unsigned int* indices = object.getIndices();

    glClearColor(0.21f, 0.3f, 0.3f, 1.0f);

    GLuint vboId, eboId, vaoId;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
    glGenBuffers(1, &eboId);
    // bind vao
    glBindVertexArray(vaoId);
    // copy vertices in vbo
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, object.nbVertices()*sizeof(float), vertices, GL_STATIC_DRAW);
    // copy indices in ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.nbIndices()*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    // set the vertex attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // draw only surrounding lines
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // MVP
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);


    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        shader.use();
        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();
        model = glm::mat4(1.0f);
        shader.setMat4fv("model", model);
        shader.setMat4fv("view", view);
        shader.setMat4fv("projection", projection);
        shader.setMat4fv("trans", trans);

        glBindVertexArray(vaoId);
        glDrawElements(GL_TRIANGLES, object.nbIndices(), GL_UNSIGNED_INT, 0);
        // unbind vao
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vaoId);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &eboId);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}


// int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
//     // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/armadillo.obj");
//     // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/garg.obj");
//     // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/chess_piece.obj");
//     // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/triangle.obj");
//     // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/cube.obj");
//     mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/venus.obj");
//     // fromObj.checkCorrectness();
//     // fromObj.toObj("media/objects/produced_chess_piece.obj");
//     // std::printf("nbVertices:%d, nbFaces:%d\n", fromObj.mNbVertices, fromObj.mNbFaces);
//     // fromObj.print();
//     // fromObj.printStats();
//     fromObj.triToQuad();
//     // fromObj.printStats();
//     // fromObj.print();
//     fromObj.toObj("bin/objects/tmp.obj");
//     exit(EXIT_SUCCESS);
// }