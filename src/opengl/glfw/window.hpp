#pragma once

#include <string>

#include "glfw.hpp"

namespace opengl{

/**
 * Initiate glfw
 * Exit program if fails
*/
void initGLFW();

/**
 * Create a window
 * @param width The window's width
 * @param height The window's height
 * @param title The window's title
 * @param resizable True if the window is resizable (default to False)
 * @param openglVersionMajor The major version of OpenGL (default 3)
 * @param openglVersionMinor The minor version of OpenGL (default 3)
*/
GLFWwindow* createWindow(int width, int height, const std::string & title, bool resizable = GLFW_FALSE, int openglVersionMajor = 3, int openglVersionMinor = 3);

}