#pragma once
#include <GLFW/glfw3.h>
#include "Manager.h"
#include <vector>
#include <unordered_map>

class InputManager : public Manager {


public: 
	enum ActionType {
		Null,
		Jump,
		Attack,
		Left,
		Right,
		Up,
		Down
	};

	struct KeyMapping {
		int key;
		ActionType action;
	};

	std::vector<KeyMapping> GetKeyMappings() {
		return keyMappings;
	}
	void ProcessAction(ActionType action);

	void ChangeKeyMapping(ActionType action, int newKey);

	void Init();

	void Update(const std::vector<bool>& inputState);

	void Save() override;

private:
	std::vector<bool> GetInputState(GLFWwindow* window);
	std::vector<KeyMapping> keyMappings;
};