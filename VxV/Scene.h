#pragma once
#include "GameObject.h"

class Scene {
private:
	std::list<GameObject*> gameObjects;

public:
	Scene(){

	}

	glm::vec3 origin = glm::vec3(0, 0, 0);

	std::list<GameObject*> GetGameObjects() {
		return gameObjects;
	}

	void AddGameObject(GameObject* go) {
		gameObjects.push_back(go);
	}
};