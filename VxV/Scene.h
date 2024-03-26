#pragma once
#include "GameObject.h"
#include <vector>

class SceneManager;


class Scene {
private:
	std::vector<GameObject*> gameObjects;
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

	void OrganizeGameObjects() {
		int n = gameObjects.size();
		bool swapped;

		if (!gameObjects.empty()) {

			do {
				swapped = false;
				for (int i = 1; i < n; ++i) {
					if (gameObjects[i - 1]->GetId() > gameObjects[i]->GetId()) {
						// Effectuer l'échange manuellement sans utiliser std::swap
						GameObject* temp = gameObjects[i - 1];
						gameObjects[i - 1] = gameObjects[i];
						gameObjects[i] = temp;
						swapped = true;
					}
				}
				--n;
			} while (swapped);
		}
	}
};