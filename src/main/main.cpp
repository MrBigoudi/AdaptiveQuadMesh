#include <cstdlib>

#include "main.hpp"

// camera
scene::Camera camera(maths::Vector3(0.0f, 0.0f, 1.0f));

const float ROTATION_ANGLE = 10.0f;
float rotationAngle = 0.0f;
scene::RotationAxe rotationAxe = scene::x;
const float LINE_WIDTH = 0.5f;
float lineWidth = 1.0f;

const char* DEFAULT_OBJECT = "media/objects/chess_piece.obj";

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

    // IMGUI
    [[maybe_unused]] ImGuiIO& io = initIMGUI(window);
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // create a file browser instance
    ImGui::FileBrowser fileDialog;
    
    // (optional) set browser properties
    fileDialog.SetTitle("Select Mesh (*.obj)");
    fileDialog.SetTypeFilters({ ".obj" });


    opengl::ResourceManager::loadShader("src/shaders/vert.glsl", "src/shaders/frag.glsl", "objectShader");

    // scene::Object* object = new scene::Object("media/objects/armadillo.obj");
    // scene::Object* object = new scene::Object("media/objects/chess_piece.obj");
    // scene::Object* object = new scene::Object("media/objects/venus.obj");
    // scene::Object* object = new scene::Object("media/objects/garg.obj");
    scene::Object* object = new scene::Object(DEFAULT_OBJECT);

    object->initCamera(&camera);

    glEnable(GL_DEPTH_TEST);


    // MVP
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);


    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glfwPollEvents();

        // glClearColor(0.21f, 0.3f, 0.3f, 1.0f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // ##################################### OBJECT ###########################################
        // MVP
        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();
        model = glm::mat4(1.0f);

        if(toQuad){
            object->toQuadMesh();
            // object.mMesh->printStats();
            object->initCamera(&camera);
            toQuad = false;
        }

        object->update(deltaTime, rotationAngle, rotationAxe, glm::vec3(1.0f));
        object->draw("objectShader", model, view, projection, lineWidth);

        // ##################################### OBJECT DONE ###########################################



        // ##################################### IMGUI ###########################################

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_another_window){
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            // open file dialog when user clicks this button
            if(ImGui::Button("Load a Mesh"))
                fileDialog.Open();
            if(ImGui::Button("Triangular To Quad"))
                object->toQuadMesh();
            ImGui::Text("\n\nZoom In: Press W\nZoom Out: Press S\n\n");
            ImGui::Text("Rotate on X axis: Press DOWN / UP\nRotate on Y axis: Press D / A\nRotate on Z axis: Press LEFT / RIGHT\n\n");
            if(ImGui::Button("Wider Lines"))
                lineWidth += LINE_WIDTH;
            if(ImGui::Button("Smaller Lines"))
                lineWidth -= LINE_WIDTH;
            if(ImGui::Button("Reset")){
                delete object;
                object = new scene::Object(DEFAULT_OBJECT);
                object->initCamera(&camera);
            }
            ImGui::End();
        }
        // file dialog
        fileDialog.Display();
        if(fileDialog.HasSelected()){
            // std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
            delete object;
            object = new scene::Object(fileDialog.GetSelected().string());
            object->initCamera(&camera);
            fileDialog.ClearSelected();
        }


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // ##################################### IMGUI DONE ###########################################


        // unbind vao
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    cleanIMGUI();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete object;

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