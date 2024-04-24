#pragma once
#include <vector>
#include <string>
#include <GLM/glm.hpp>

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

	std::vector<GameObject*>& GetGameObjects() {
		return gameObjects;
	}

	void AddGameObject(GameObject* go) {
		gameObjects.push_back(go);
		allGameObjects.push_back(go);
	}

	void RemoveGameObject(int id) {
		for (int i = 0; i < allGameObjects.size(); i++)
		{
			if (allGameObjects[i]->GetId() == id) {
				delete(allGameObjects[i]);
				break;
			}
		}
	}

	std::vector<GameObject*> GetAllGameObjects() {
		return allGameObjects;
	}

	void Init() {
		for (GameObject* go : gameObjects) {
			go->Init();
		}
	}

	void Start() {
		for (GameObject* go : gameObjects) {
			go->Start();
		}
	}

	void Update() {
		for (GameObject* go : gameObjects) {
			go->Update();
		}
	}
};