#pragma once
#include <vector>
#include <string>
#include <GLM/glm.hpp>


#include "GameObject.h"

class SceneManager;


class Scene {
private:
	std::vector<GameObject*> gameObjects;
	static std::vector<GameObject*> allGameObjects;
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

	static std::vector<GameObject*> GetAllGameObjects() {
		return allGameObjects;
	}

};