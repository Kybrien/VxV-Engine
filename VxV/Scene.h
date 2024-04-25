#pragma once
#include <vector>
#include <string>
#include <GLM/glm.hpp>

#include "GameObject.h"

class SceneManager;


class Scene {
private:
	/**
	* @brief Vector containing pointers to the parents GameObjects in the current scene.
	* Represents the GameObjects present in the current scene.
	*/
	std::vector<GameObject*> gameObjects;

	/**
	* @brief Vector containing pointers to all GameObjects in the scene.
	* Represents all GameObjects present in the scene.
	*/
	std::vector<GameObject*> allGameObjects;

	/**
	* @brief Pointer to the SceneManager managing the scene.
	* Represents the SceneManager responsible for managing the scene.
	*/
	SceneManager* sceneManager;


public:
	/**
	* @brief Constructs a Scene object with an optional name.
	* @param name_ The name of the scene. Default is "SampleScene".
	* Represents constructing a Scene object with an optional name.
	*/
	Scene(std::string name_ = "SampleScene");

	/**
	* @brief Represents the name of the scene.
	* Stores the name assigned to the scene.
	*/
	std::string name;

	/**
	* @brief Represents the origin point of the scene.
	* Stores the origin point of the scene.
	*/
	glm::vec3 origin;

	/**
	* @brief Retrieves a reference to the vector containing pointers to GameObjects in the scene.
	* @return A reference to the vector containing pointers to GameObjects in the scene.
	* Represents retrieving the vector of GameObjects in the scene.
	*/
	std::vector<GameObject*>& GetGameObjects() {
		return gameObjects;
	}

	/**
	* @brief Adds a GameObject to the scene.
	* @param go Pointer to the GameObject to add.
	* Represents adding a GameObject to the scene.
	*/
	void AddGameObject(GameObject* go) {
		gameObjects.push_back(go);
		allGameObjects.push_back(go);
	}

	/**
	* @brief Removes a GameObject from the scene by its ID.
	* @param id The ID of the GameObject to remove.
	* Represents removing a GameObject from the scene by its ID.
	*/
	void RemoveGameObject(int id) {
		for (int i = 0; i < allGameObjects.size(); i++)
		{
			if (allGameObjects[i]->GetId() == id) {
				delete(allGameObjects[i]);
				break;
			}
		}
	}


	/**
	* @brief Retrieves all GameObjects in the scene.
	* @return A vector containing pointers to all GameObjects in the scene.
	* Represents retrieving all GameObjects in the scene.
	*/
	std::vector<GameObject*> GetAllGameObjects() {
		return allGameObjects;
	}

	/**
	* @brief Initializes all GameObjects in the scene.
	* Represents initializing all GameObjects in the scene.
	*/
	void Init() {
		for (GameObject* go : gameObjects) {
			go->Init();
		}
	}

	/**
	* @brief Starts all GameObjects in the scene.
	* Represents starting all GameObjects in the scene.
	*/
	void Start() {
		for (GameObject* go : gameObjects) {
			go->Start();
		}
	}

	/**
	* @brief Updates all GameObjects in the scene.
	* Represents updating all GameObjects in the scene.
	*/
	void Update() {
		for (GameObject* go : gameObjects) {
			go->Update();
		}
	}

};