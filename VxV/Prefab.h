#pragma once
#include "GameObject.h"

class Prefab {
private: 
	/**
	* @brief Pointer to the GameObject associated with the Prefab.
	* Represents the GameObject associated with the Prefab.
	*/
	GameObject* gameObject;


public:
	/**
	* @brief Represents the name of the prefab.
	* Stores the name assigned to the prefab.
	*/
	std::string name;

	/**
	* @brief Constructs a Prefab object with optional parameters.
	* @param go Pointer to the GameObject used to create the prefab.
	* @param name The name of the prefab. Default is "PrefabSample".
	* Represents constructing a Prefab object with optional parameters.
	*/
	Prefab(GameObject* go, std::string name = "PrefabSample");

	/**
	* @brief Retrieves the GameObject associated with the Prefab.
	* @return Pointer to the GameObject associated with the Prefab.
	* Represents retrieving the GameObject associated with the Prefab.
	*/
	GameObject* getGameObject() {
		return gameObject;
	}

	/**
	* @brief Creates a copy of the provided GameObject and fills it with data from another GameObject.
	* @param goToFill Pointer to the GameObject to fill with data.
	* @param goToCopy Pointer to the GameObject to copy data from.
	* @return Pointer to the filled GameObject copy.
	* Represents creating a copy of a GameObject and filling it with data from another GameObject.
	*/
	static GameObject* Copy(GameObject* goToFill, GameObject* goToCopy);

};