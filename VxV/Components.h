#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Initialisation de GLFW et création d'une fenêtre
GLFWwindow* initWindow(const int width, const int height) {
    if (!glfwInit()) {
        std::cerr << "Erreur lors de l'initialisation de GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Moteur de Jeu", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    return window;
}

// Initialisation de GLEW
bool initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erreur lors de l'initialisation de GLEW" << std::endl;
        return false;
    }
    return true;
}

int main() {
    GLFWwindow* window = initWindow(800, 600);
    if (!window) return -1;

    if (!initGLEW()) {
        glfwTerminate();
        return -1;
    }

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        // Gestion des entrées
        processInput(window);

        // Logique de mise à jour

        // Rendu
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers et poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
