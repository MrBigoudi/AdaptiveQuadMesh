#include "glad_init.hpp"
#include "glfw/glfw.hpp"
#include <cstdlib>

void opengl::initGLAD(){
    if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ){
        fprintf(stderr, "Failed to initialize GLAD!\n");
        exit(EXIT_FAILURE);
    }
}