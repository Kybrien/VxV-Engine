#pragma once
#include "GameObject.h"

class SceneManager;


class Scene {
private:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> allGameObjects;
	SceneManager* sceneManager;

public:
	Scene(std::string name_ = "SampleScene");

	std::string name;
	glm::vec3 origin;

	std::vector<GameObject*> GetGameObjects() {
		return gameObjects;
	}

	void AddGameObject(GameObject* go) {
		gameObjects.push_back(go);
	}
};