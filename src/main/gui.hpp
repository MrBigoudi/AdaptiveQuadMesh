#pragma once

#include "scene.hpp"
#include "opengl.hpp"

/**
 * Initiate imgui
 * @param window The glfw's window
 * @return The io
*/
ImGuiIO& initIMGUI(GLFWwindow* window);

/**
 * Quit imgui
*/
void cleanIMGUI();