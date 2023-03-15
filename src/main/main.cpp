#include <cstdlib>

#include "mesh.hpp"
#include "scene.hpp"
#include "opengl.hpp"
#include "utils.hpp"
#include "maths.hpp"

// camera
scene::Camera camera(maths::Vector3(0.0f, 0.0f, 1.0f));

const float ROTATION_ANGLE = 10.0f;
float rotationAngle = 0.0f;
scene::RotationAxe rotationAxe = scene::x;
float lineWidth = 1.0f;

bool toQuad = false;

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
    // close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // move the camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(scene::CAM_BACKWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.processKeyboardInput(scene::CAM_LEFT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.processKeyboardInput(scene::CAM_RIGHT, deltaTime);

    // rotate the object
    rotationAngle = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        rotationAngle = -ROTATION_ANGLE;
        rotationAxe = scene::z;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        rotationAngle = ROTATION_ANGLE;
        rotationAxe = scene::z;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        rotationAngle = -ROTATION_ANGLE;
        rotationAxe = scene::x;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        rotationAngle = ROTATION_ANGLE;
        rotationAxe = scene::x;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        rotationAngle = -ROTATION_ANGLE;
        rotationAxe = scene::y;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        rotationAngle = ROTATION_ANGLE;
        rotationAxe = scene::y;
    }

    // transform the object in a quad
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) toQuad = true;

    // modify line width
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) lineWidth += 0.1f;
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) lineWidth -= 0.1f;

}


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){

    opengl::initGLFW();
    GLFWwindow* window = opengl::createWindow(SCR_WIDTH, SCR_HEIGHT, "AdaptiveQuadMesh", GLFW_TRUE);
    opengl::initGLAD();
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    opengl::ResourceManager::loadShader("src/shaders/vert.glsl", "src/shaders/frag.glsl", "objectShader");

    // scene::Object object = scene::Object("media/objects/armadillo.obj");
    // scene::Object object = scene::Object("media/objects/chess_piece.obj");
    // scene::Object object = scene::Object("media/objects/venus.obj");
    scene::Object object = scene::Object("media/objects/garg.obj");

    object.initCamera(&camera);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.21f, 0.3f, 0.3f, 1.0f);

    // MVP
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);


    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glfwPollEvents();

        if(toQuad){
            object.toQuadMesh();
            // object.mMesh->printStats();
            object.initCamera(&camera);
            toQuad = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // MVP
        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();
        model = glm::mat4(1.0f);

        object.update(deltaTime, rotationAngle, rotationAxe, glm::vec3(1.0f));
        object.draw("objectShader", model, view, projection, lineWidth);

        // unbind vao
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

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