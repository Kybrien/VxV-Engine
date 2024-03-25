#pragma once
#include "GameObject.h"

class SceneManager;


class Scene {
private:
	std::list<GameObject*> gameObjects;
	SceneManager* sceneManager;

public:
	Scene(std::string name_ = "SampleScene");

	std::string name;
	glm::vec3 origin;

	std::list<GameObject*> GetGameObjects() {
		return gameObjects;
	}

	void AddGameObject(GameObject* go) {
		gameObjects.push_back(go);
	}
};