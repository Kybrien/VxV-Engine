#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <iostream>

void InputManager::ProcessAction(ActionType action)
{
    switch (action) {
    case ActionType::Jump:
        std::cout << "jump" << std::endl;
        break;
    case ActionType::Attack:
        std::cout << "attack" << std::endl;
        break;
    case ActionType::Left:
        std::cout << "left" << std::endl;
        break;
    case ActionType::Right:
        std::cout << "right" << std::endl;
        break;
    case ActionType::Up:
        std::cout << "up" << std::endl;
        break;
    case ActionType::Down:
        std::cout << "down" << std::endl;
        break;
       }
}

void InputManager::Init() {
    GetKeyMappings();
    keyMappings = {
        {GLFW_KEY_SPACE, ActionType::Jump},
        {GLFW_KEY_X, ActionType::Attack},
        {GLFW_KEY_LEFT, ActionType::Left},
        {GLFW_KEY_RIGHT, ActionType::Right},
        {GLFW_KEY_UP, ActionType::Up},
        {GLFW_KEY_DOWN, ActionType::Down}
    };
}

std::vector<bool> InputManager::GetInputState(GLFWwindow* window) {
    std::vector<bool> inputState;

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        inputState.push_back(glfwGetKey(window, key) == GLFW_PRESS);
    }

    return inputState;
}

void InputManager::ChangeKeyMapping(ActionType action, int newKey) {
    // Recherche du mapping correspondant à l'action spécifiée
    auto it = std::find_if(keyMappings.begin(), keyMappings.end(),
        [&action](const KeyMapping& mapping) { return mapping.action == action; });

    // Si le mapping est trouvé, modifier la touche associée
    if (it != keyMappings.end()) {
        it->key = newKey;
        std::cout << "Key mapping updated for action " << static_cast<int>(action) << std::endl;
    }
    else {
        std::cout << "Action not found." << std::endl;
    }
}

void InputManager::Update(const std::vector<bool>& inputState)
{
    for (const auto& mapping : keyMappings) {
        int key = mapping.key; // Utilisez 'key' au lieu de 'first'
        ActionType action = mapping.action; // Utilisez 'action' au lieu de 'second'
        // Vérifie si la touche est enfoncée dans l'état d'entrée actuel
        if (inputState[key] == GLFW_PRESS) {
            // Traiter l'action correspondante
            ProcessAction(action);
        }
    }
}