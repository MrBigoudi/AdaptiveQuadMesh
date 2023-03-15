#include <cstdlib>
#include "window.hpp"

namespace opengl{

void initGLFW(){
    if(glfwInit() != GLFW_TRUE){
        fprintf(stderr, "Failed to init GLFW!\n");
        exit(EXIT_FAILURE);
    }
}

GLFWwindow* createWindow(int width, int height, const std::string & title, bool resizable, int openglVersionMajor, int openglVersionMinor){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if( !window ) {
        fprintf(stderr, "Failed to create a window!\n");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    return window;

}

}