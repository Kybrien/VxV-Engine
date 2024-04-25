#ifndef CONTROLS_HPP
#define CONTROLS_HPP
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
* @brief Handle the inputs of the user and change the view matrix in consequence
* @param window The window
*/
void computeMatricesFromInputs(GLFWwindow* window);

/**
* @brief Get the view matrix
*/
glm::mat4 getViewMatrix();

/**
* @brief Get the projection matrix
*/
glm::mat4 getProjectionMatrix();

#endif
